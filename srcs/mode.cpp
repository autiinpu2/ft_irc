#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"


# include <sys/socket.h>

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