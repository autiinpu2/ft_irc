/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:51:32 by mathys            #+#    #+#             */
/*   Updated: 2026/10/08 01:16:58 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Parsing.hpp"
#include "utils.hpp"

# include <algorithm>
# include <sstream>
# include <string>
# include <sys/socket.h>

static void sendMsg(Client *c, const std::string &msg) {
	std::string m = msg + "\r\n";
	send(c->getFd(), m.c_str(), m.length(), 0);
}

Command::Command(Server &server) : _server(server) {}

Command::~Command() {}

void Command::handleCmd(const std::string &buffer, Client *c) {
	std::stringstream 	stream(buffer);
	std::string 		line;

	while (std::getline(stream, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			continue;

		std::string command;
		std::vector<std::string> arg;
		Parsing::parseLine(line, command, arg);

		LOG_STATUS status= c->getStatus();
		if (command == "CAP")
			continue;
		else if (command == "PASS" && status == NONE)
			this->cmdPass(arg, c);
		else if (command == "NICK" && (status == PASSWORD || status == USERNAME))
			this->cmdNick(arg, c, false);
		else if (command == "USER" && (status == PASSWORD || status == NICKNAME))
			this->cmdUser(arg, c);
		else if (c->getStatus() == FULL) {
			if (command == "PING")
				this->cmdPing(arg, c);
			else if (command == "NICK")
				this->cmdNick(arg, c, true);
			else if (command == "USER")
				this->cmdUser(arg, c);
			else if (command == "JOIN")
				this->cmdJoin(arg, c);
			else if (command == "PRIVMSG")
				this->cmdMsg(arg, c);
			else if (command == "MODE")
				this->cmdMode(arg, c);
			else if (command == "INVITE")
				this->cmdInvite(arg, c);
			else if (command == "TOPIC")
				this->cmdTopic(arg, c);
		}
		else if (status != FULL)
			sendMsg(c, ":ircserv 451 * :You have not registered");

	}
}

void Command::cmdPass(std::vector<std::string> arg, Client *c) {
	if (arg.empty()) {
		std::string err = ":ircserv 461 * :No password given\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}
	if (arg[0] == this->_server.getPassword()) {
		c->setStatus(PASSWORD);
		std::cout << "\033[1;32mNew client logged!\033[0m" << std::endl;
	}
	else {
		std::string msg = ":ircserv 464 * :Password incorrect\r\n";
		send(c->getFd(), msg.c_str(), msg.length(), 0);
	}
}

void Command::cmdNick(std::vector<std::string> nick, Client *c, bool is_logged) {
	if (nick.empty()) {
		std::string err = ":ircserv 431 * :No nickname given\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	std::vector<std::string> &used_nicks = this->_server.getUsedNicks();
	std::vector<std::string>::iterator it = std::find(used_nicks.begin(), used_nicks.end(), nick[0]);

	if (it != used_nicks.end()) {
		std::string msg = ":ircserv 433 " + c->getNick() + " " + nick[0] + " :Nickname is already in use\r\n";
		send(c->getFd(), msg.c_str(), msg.length(), 0);
	}
	else {
		std::string old_nick = c->getNick();
		used_nicks.push_back(nick[0]);
		c->setNickname(nick[0]);
		if (is_logged) {
			if (!old_nick.empty()) {
				std::vector<std::string>::iterator it_old = std::find(used_nicks.begin(), used_nicks.end(), old_nick);
				if (it_old != used_nicks.end())
					used_nicks.erase(it_old);
			}
			std::string success_msg = ":" + old_nick + "!user@ircserv NICK :" + nick[0] + "\r\n";
			send(c->getFd(), success_msg.c_str(), success_msg.length(), 0);
		}
		else if (c->getStatus() == USERNAME) {
			c->setStatus(FULL);
			std::string welcome = ":ircserv 001 " + c->getNick() + " :Welcome to the IRC Network\r\n";
			send(c->getFd(), welcome.c_str(), welcome.length(), 0);
		}
		else {
			c->setStatus(NICKNAME);
		}
	}
}

void Command::cmdUser(std::vector<std::string> arg, Client *c) {
	if (arg.size() < 4) {
		std::string err = ":ircserv 461 * USER :Not enough parameters\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	if (c->getStatus() == FULL || c->getStatus() == USERNAME) {
		std::string err = ":ircserv 462 " + c->getRealname() + " :You may not reregister\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	c->setUsername(arg[0]);
	c->setRealname(arg[3]);

	if (c->getStatus() == NICKNAME) {
		c->setStatus(FULL);
		std::string welcome = ":ircserv 001 " + c->getNick() + " :Welcome to the IRC Network\r\n";
		send(c->getFd(), welcome.c_str(), welcome.length(), 0);
	} else {
		c->setStatus(USERNAME);
	}
}

void Command::cmdPing(std::vector<std::string> arg, Client *c) {
	if (arg.empty())
		return;
	std::string msg = ":ircserv PONG * :" + arg[0] + "\r\n";
	send(c->getFd(), msg.c_str(), msg.length(), 0);
}

static std::string joinError(Channel *ch, Client *c, const std::string &key) {
	std::string target = " " + c->getNick() + " " + ch->getName();

	if (ch->isInviteOnly() && !ch->isInvited(c))
		return ":ircserv 473" + target + " :Cannot join channel (+i)";
	if (!ch->getKey().empty() && ch->getKey() != key)
		return ":ircserv 475" + target + " :Cannot join channel (+k)";
	if (ch->getLimit() > 0 && ch->getSize() >= ch->getLimit())
		return ":ircserv 471" + target + " :Cannot join channel (+l)";
	return "";
}

void Command::joinChannel(const std::string &name, const std::string &key, Client *c) {
	const std::map<std::string, Channel*> &channels = this->_server.getChannel();
	std::map<std::string, Channel*>::const_iterator it = channels.find(name);

	if (name.size() < 2 || name[0] != '#' || name.size() > 200 || name.find("^G") != std::string::npos) {
		sendMsg(c, ":ircserv 403 " + c->getNick() + " " + name + " :No such channel");
		return;
	}
	if (it == channels.end()) {
		Channel *newChannel = new Channel(name, c, key);
		this->_server.addChannel(name, newChannel);
		c->setNbChannels(1);
		newChannel->returnJOIN(c);	
		return;
	}
	Channel *channel = it->second;
	if (channel->inChannel(c))
		return;
	std::string error = joinError(channel, c, key);
	if (!error.empty()) {
		sendMsg(c, error);
		return;
	}
	channel->uninvite(c);
	channel->addClient(c);
	c->setNbChannels(1);
	channel->returnJOIN(c);
}

void Command::cmdMsg(std::vector<std::string> arg, Client *c) {
	std::string msg = arg.back();
	arg.pop_back();

	std::vector<std::string> targets;
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); ++it) {
		std::stringstream ss(*it);
		std::string target;
		while (std::getline(ss, target, ',')) {
			if (!target.empty()) {
				targets.push_back(target);
			}
		}
	}

	std::vector<Client*> clients = this->_server.getClients();

	for (std::vector<std::string>::iterator iter = targets.begin(); iter != targets.end(); ++iter) {
		if (!iter->empty() && (*iter)[0] == '#')
		{
			std::map<std::string, Channel*> channels = this->_server.getChannel();
			std::map<std::string, Channel*>::iterator it = channels.find(*iter);

			if (it != channels.end()) {
				Channel* chan = it->second;
				chan->broadcast(msg, c);
			} else {
				std::string send_msg = ":ircserv 403 " + c->getNick() + " " + *iter + " :No such channel\r\n";
				send(c->getFd(), send_msg.c_str(), send_msg.length(), 0);
			}
		}
		else {
			Client* recv = NULL;

			for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
				Client* currentClient = *it;
				if (currentClient != NULL && currentClient->getNick() == *iter) {
					recv = currentClient;
					break;
				}
			}

			if (recv != NULL) {
				std::string send_msg = ":" + c->getNick() + " PRIVMSG " + recv->getNick() + " :" + msg + "\r\n";
				send(recv->getFd(), send_msg.c_str(), send_msg.length(), 0);
			} else {
				std::string send_msg = ":ircserv 401 " + c->getNick() + " " + *iter + " :No such nick/channel\r\n";
				send(c->getFd(), send_msg.c_str(), send_msg.length(), 0);
			}
		}
	}
}

void Command::cmdJoin(std::vector<std::string> args, Client *c) {
	if (args.empty()) {
		sendMsg(c, ":ircserv 461 " + c->getNick() + " JOIN :Not enough parameters");
		return;
	}

	std::vector<std::string> names = Parsing::splitArg(args[0]);
	std::vector<std::string> keys;

	if (args.size() > 1)
		keys = Parsing::splitArg(args[1]);

	if (Parsing::vectorEmpty(names) || Parsing::vectorEmpty(keys)) {
		sendMsg(c, ":ircserv 403 " + c->getNick() + " " + args[0] + " :No such channel");
		return;
	}

	for (size_t i = 0; i < names.size(); ++i) {
		if (i < keys.size())
			this->joinChannel(names[i], keys[i], c);
		else
			this->joinChannel(names[i], "", c);
	}
}

void Command::cmdMode(std::vector<std::string> arg, Client *c) {
	if (arg.size() == 0) {
		err_msg(461, "MODE :Not enough parameters", c);
		return;
	}
	Channel *chan;
	std::map<std::string, Channel*> serv_chan = this->_server.getChannel();
	std::map<std::string, Channel*>::iterator iter = serv_chan.find(arg[0]);

	if (iter != serv_chan.end()) {
		chan = iter->second;
	} else {
		err_msg(403, arg[0] + " :No such channel", c);
		return;
	}

	if (!chan->inChannel(c)) {
		err_msg(442, chan->getName() + " :You're not on that channel", c);
		return;
	}
	if (arg.size() == 1) {
		std::string modes = "+";
		std::string arguments;
		for (std::set<char>::iterator it = chan->getMode().begin(); it != chan->getMode().end(); ++it) {
			modes += *it;
			if (*it == 'k') {
				arguments += " " + chan->getKey();
			} else if (*it == 'l') {
				std::ostringstream oss;
				oss << chan->getLimit();
				arguments += " " + oss.str();
			}
		}
		std::string msg = ":ircserv 324 " + c->getNick() + " " + chan->getName() + " " + modes + arguments + "\r\n";
		std::cout << "[" << msg << "]";
		send(c->getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	std::vector<ChannelMode> parsed_modes;
	char current_sign = '+';
	size_t arg_index = 2;

	for (size_t k = 1; k < arg.size(); ++k) {
		if (arg[k][0] == '+' || arg[k][0] == '-') {
			for (size_t i = 0; i < arg[k].length(); ++i) {
				char ch = arg[k][i];
				if (ch == '+' || ch == '-') {
					current_sign = ch;
				} else {
					ChannelMode m;
					if (current_sign == '+')
						m.sign = ADD;
					else
						m.sign = REMOVE;
					m.mode = ch;
					m.param = "";
					if ((ch == 'l' && current_sign == '+') || (ch == 'k') || (ch == 'o')) {
						if (arg_index < arg.size()) {
							m.param = arg[arg_index++];
						} else {
							err_msg(461, "MODE :Not enough parameters", c);
							continue;
						}
					}
					parsed_modes.push_back(m);
				}
			}
		}
	}

	for (size_t i = 0; i < parsed_modes.size(); ++i) {
		ChannelMode mod = parsed_modes[i];
		std::string param = parsed_modes[i].param;

		if (mod.mode == 'i')
			cmdModeI(mod, chan, c);
		else if (mod.mode == 't')
			cmdModeT(mod, chan, c);
		else if (mod.mode == 'k')
			cmdModeK(mod, chan, c);
		else if (mod.mode == 'o')
			cmdModeO(mod, chan, c);
		else if (mod.mode == 'l')
			cmdModeL(mod, chan, c);
		else
			err_msg(472, std::string(1, mod.mode) + " :is unknown mode char to me for " + chan->getName(), c);
	}
}

void Command::cmdModeI(ChannelMode mode, Channel *chan, Client *c) {
	if (chan->isOperator(c)) {
		if (mode.sign == ADD) {
			chan->setInviteOnly(true);
			chan->addMode('i');
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " +i";
			chan->replyALL(msg);
		}
		else if (mode.sign == REMOVE) {
			chan->setInviteOnly(false);
			chan->removeMode('i');
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " -i";
			chan->replyALL(msg);
		}
	} else {
		err_msg(482, chan->getName() + " :You're not channel operator", c);
	}
}

void Command::cmdModeL(ChannelMode mode, Channel *chan, Client *c) {
	if (chan->isOperator(c)) {
		if (mode.sign == ADD) {
			chan->setLimit(stringToSizeT(mode.param));
			chan->addMode('l');
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " +l " + mode.param;
			chan->replyALL(msg);
		}
		else {
			chan->setLimit(0);
			chan->removeMode('l');
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " -l ";
			chan->replyALL(msg);
		}
	} else {
		err_msg(482, chan->getName() + " :You're not channel operator", c);
		return ;
	}
}

void Command::cmdModeT(ChannelMode mode, Channel *chan, Client *c) {
	if (chan->isOperator(c)) {
		if (mode.sign == ADD) {
			chan->setTopicOperator(true);
			chan->addMode('t');
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " +t";
			chan->replyALL(msg);
		}
		else {
			chan->setTopicOperator(false);
			chan->removeMode('t');
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " -t";
			chan->replyALL(msg);
		}
	} else {
		err_msg(482, chan->getName() + " :You're not channel operator", c);
		return ;
	}
}

void Command::cmdModeO(ChannelMode mode, Channel *chan, Client *c) {
	if (chan->isOperator(c)) {
		std::string target_nick = mode.param;
		std::vector<Client*> clients = this->_server.getClients();
		Client *target = NULL;
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
			Client* currentClient = *it;
			if (currentClient != NULL && currentClient->getNick() == target_nick) {
				target = currentClient;
				break;
			}
		}
		if (target == NULL) {
			err_msg(401, target_nick + " :No such nick/channel", c);
			return ;
		}
		if (!chan->inChannel(target)) {
			err_msg(441, mode.param + " " + chan->getName() + " :They aren't on that channel", c);
			return ;
		}
		if (mode.sign == ADD) {
			chan->addOperator(target);
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " +o " + target_nick;
			chan->replyALL(msg);
		}
		else {
			chan->removeOperator(target);
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " -o " + target_nick;
			chan->replyALL(msg);
		}
	} else {
		err_msg(482, chan->getName() + " :You're not channel operator", c);
		return ;
	}
}

void Command::cmdModeK(ChannelMode mode, Channel *chan, Client *c) {
	std::string password = mode.param;
	if (chan->isOperator(c)) {
		if (mode.sign == ADD) {
			chan->setKey(password);
			chan->addMode('k');
			std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " +k " + password;
			chan->replyALL(msg);
		} else {
			if (chan->getKey() != password) {
				err_msg(467, chan->getName() + " :Channel key already set", c);
			} else {
				chan->setKey("");
				chan->removeMode('k');
				std::string msg = ":" + c->getNick() + "!" + c->getUser() + "@ircserv MODE " + chan->getName() + " -k";
				chan->replyALL(msg);
			}
		}
	} else {
		err_msg(482, chan->getName() + " :You're not channel operator", c);
		return ;
	}
}

void	Command::cmdTopic(std::vector<std::string> arg, Client *c) {
	if (arg.size() == 0) {
		err_msg(461, "TOPIC :Not enough parameters", c);
		return ;
	}
	Channel *channel;
	std::string channelName = arg[0];
	std::map<std::string, Channel*> serv_chan = this->_server.getChannel();
	std::map<std::string, Channel*>::iterator iter = serv_chan.find(channelName);

	if (iter != serv_chan.end()) {
		channel = iter->second;
	} else {
		err_msg(403, channelName + " :No such channel", c);
		return ;
	}
	if (!channel->inChannel(c)) {
		err_msg(442, channelName + " :You're not on that channel", c);
		return ;
	}
	if (arg.size() == 1) {
		if (channel->getTopic().empty()) {
			std::string msg = ":ircserv 331 "+ c->getNick() + " " + channel->getName() + " :No topic is set\r\n";
			send(c->getFd(), msg.c_str(), msg.length(), 0);
		} else {
			std::string msg = ":ircserv 332 "+ c->getNick() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n";
			send(c->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
	else {
		if (channel->getTopicOperator() && !channel->isOperator(c)) {
			err_msg(482, channel->getName() + " :You're not channel operator", c);
			return;
		}
		std::string topic_name = arg[1];
		if (topic_name.empty()) {
			channel->setTopic("");
			std::string msg = ":" + c->getNick() + "!user@ircserv TOPIC " + channel->getName() + " :" + channel->getTopic() + "\r\n";
			channel->replyALL(msg);
		} else {
			channel->setTopic(topic_name);
			std::string msg = ":" + c->getNick() + "!user@ircserv TOPIC " + channel->getName() + " :" + channel->getTopic();
			channel->replyALL(msg);
		}
	}
}

void	Command::cmdInvite(std::vector<std::string> arg, Client *c) {
	if (arg.size() != 2)
	{
		err_msg(461, "INVITE :Not enough parameters", c);
		return ;
	}

	std::string nickname = arg[0];
	std::string channel_name = arg[1];

	Client *client = NULL;
	Channel *channel;

	std::map<std::string, Channel*> serv_chan = this->_server.getChannel();
	std::map<std::string, Channel*>::iterator iter = serv_chan.find(channel_name);

	std::vector<Client*> connected_clients = this->_server.getClients();
	for (std::vector<Client*>::iterator it = connected_clients.begin(); it != connected_clients.end(); ++it) {
		if ((*it)->getNick() == nickname) {
			client = *it;
			break;
		}
	}
	if (client == NULL) {
		err_msg(401, nickname + " :No such nick/channel", c);
		return ;
	}

	if (iter != serv_chan.end()) {
		channel = iter->second;
	} else {
		err_msg(403, channel_name + " :No such channel", c);
		return ;
	}
	if (!channel->inChannel(c)) {
		err_msg(442, channel_name + " :You're not on that channel", c);
		return ;
	}
	if (!channel->isOperator(c)) {
		err_msg(482, channel_name + " :You're not channel operator", c);
		return ;
	}
	if (channel->inChannel(client)) {
		err_msg(443, nickname + " " + channel_name + " :is already on channel", c);
		return ;
	}
	channel->invite(client);
	err_msg(341, nickname + " " + channel_name, c);
	std::string msg = ":" + c->getNick() + "!user@ircserv INVITE " + nickname + " :" + channel_name + "\r\n";
	send(client->getFd(), msg.c_str(), msg.length(), 0);
}
