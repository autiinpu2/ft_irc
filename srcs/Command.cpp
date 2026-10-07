/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:51:32 by mathys            #+#    #+#             */
/*   Updated: 2026/10/06 03:25:53 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

# include <algorithm>
# include <sstream>
# include <string>
# include <sys/socket.h>

Command::Command(Server &server) : _server(server) {}

Command::~Command() {}

void Command::handleCmd(const std::string &buffer, Client *c) {
	std::stringstream stream(buffer);
	std::string line;
	
	while (std::getline(stream, line)) {
		if (line.empty())
			continue;
		if (line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);

		std::stringstream lineStream(line);
		std::string command;
		lineStream >> command;

		std::vector<std::string> arg;
		std::string token;

		while (lineStream >> token) {
			if (token[0] == ':') {
				token.erase(0, 1);

				std::string rest;
				std::getline(lineStream, rest);

				token += rest;
				arg.push_back(token);
				break;
			} 
            else {
				arg.push_back(token);
			}
		}

		if (command == "PASS" && c->getStatus() == NONE)
			this->cmdPass(arg, c);
		else if (command == "NICK" && (c->getStatus() == PASSWORD || c->getStatus() == USERNAME))
			this->cmdNick(arg, c, false);
		else if (command == "USER" && (c->getStatus() == PASSWORD || c->getStatus() == NICKNAME))
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
		}
		else {
			std::string msg = ":localhost 451 * :You have not registered\r\n";
			send(c->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}

void Command::cmdPass(std::vector<std::string> arg, Client *c) {
	if (arg.empty()) {
		std::string err = ":localhost 461 * :No password given\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}
	if (arg[0] == this->_server.getPassword()) {
		c->setStatus(PASSWORD);
		std::cout << "New client logged" << std::endl;
	}
	else {
		std::string msg = ":localhost 464 * :Password incorrect\r\n";
		send(c->getFd(), msg.c_str(), msg.length(), 0);
	}
}

void Command::cmdNick(std::vector<std::string> nick, Client *c, bool is_logged) {
	if (nick.empty()) {
		std::string err = ":localhost 431 * :No nickname given\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	std::vector<std::string> &used_nicks = this->_server.getUsedNicks();
	std::vector<std::string>::iterator it = std::find(used_nicks.begin(), used_nicks.end(), nick[0]);

	if (it != used_nicks.end()) {
		std::string msg = ":localhost 433 " + c->getNick() + " " + nick[0] + " :Nickname is already in use\r\n";
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
			std::string success_msg = ":" + old_nick + "!user@localhost NICK :" + nick[0] + "\r\n";
			send(c->getFd(), success_msg.c_str(), success_msg.length(), 0);
		}
		else if (c->getStatus() == USERNAME) {
			c->setStatus(FULL);
			std::string welcome = ":localhost 001 " + c->getNick() + " :Welcome to the IRC Network\r\n";
			send(c->getFd(), welcome.c_str(), welcome.length(), 0);
		}
		else {
			c->setStatus(NICKNAME);
		}
	}
}

void Command::cmdUser(std::vector<std::string> arg, Client *c) {
	if (arg.size() < 4) {
		std::string err = ":localhost 461 * USER :Not enough parameters\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	if (c->getStatus() == FULL || c->getStatus() == USERNAME) {
		std::string err = ":localhost 462 " + c->getRealname() + " :You may not reregister\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	c->setUsername(arg[0]);
	c->setRealname(arg[3]);

	if (c->getStatus() == NICKNAME) {
		c->setStatus(FULL);
		std::string welcome = ":localhost 001 " + c->getNick() + " :Welcome to the IRC Network\r\n";
		send(c->getFd(), welcome.c_str(), welcome.length(), 0);
	} else {
		c->setStatus(USERNAME);
	}
}

void Command::cmdPing(std::vector<std::string> arg, Client *c) {
	if (arg.empty())
		return;
	std::string msg = ":localhost PONG * :" + arg[0] + "\r\n";
	send(c->getFd(), msg.c_str(), msg.length(), 0);
}

void	Command::cmdJoin(std::vector<std::string> arg, Client *c) {
	std::map<std::string, Channel*>::const_iterator it = this->_server.getChannel().find(arg[0]);
	if (it == this->_server.getChannel().end()) {
		Channel *newChannel = new Channel(arg[0], c);
		this->_server.addChannel(arg[0], newChannel);
		std::string prefix = ":" + c->getNick() + "!" + c->getUser();
		reply(c, prefix + " JOIN " + newChannel->getName()); 
		reply(c, ":ircserv MODE " + newChannel->getName() + "+nt");
		reply(c, ":ircserv 353 " + c->getNick() + " = " + newChannel->getName() + "+nt");
	}
	else {	
	}
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
				chan->broadcast(":" + c->getNick() + " PRIVMSG " + chan->getName() + " :" + msg + "\r\n", c);
			} else {
				std::string send_msg = ":localhost 403 " + c->getNick() + " " + *iter + " :No such channel\r\n";
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
				std::string send_msg = ":localhost 401 " + c->getNick() + " " + *iter + " :No such nick/channel\r\n";
				send(c->getFd(), send_msg.c_str(), send_msg.length(), 0);
			}
		}
	}
}

void Command::reply(Client *c, const std::string &msg) {
	std::string m = msg + "\r\n";
	send(c->getFd(), m.c_str(), m.length(), 0);
}
