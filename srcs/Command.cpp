/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:51:32 by mathys            #+#    #+#             */
/*   Updated: 2026/09/25 06:27:17 by mathys           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"

# include <algorithm>
# include <sstream>
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
			this->cmd_pass(arg, c);
		else if (command == "NICK" && (c->getStatus() == PASSWORD || c->getStatus() == USERNAME))
			this->cmd_nick(arg, c, false);
		else if (command == "USER" && (c->getStatus() == PASSWORD || c->getStatus() == NICKNAME))
			this->cmd_user(arg, c);
		else if (c->getStatus() == FULL) {
			if (command == "PING")
				this->cmd_ping(arg, c);
			else if (command == "NICK")
				this->cmd_nick(arg, c, true);
			else if (command == "USER")
			    this->cmd_user(arg, c);
			// else if (command == "JOIN")
			// 	this->cmd_join(arg, c);
		}
		else {
			std::string msg = ":localhost 451 * :You have not registered\r\n";
			send(c->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}

void Command::cmd_pass(std::vector<std::string> arg, Client *c) {
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

void Command::cmd_nick(std::vector<std::string> nick, Client *c, bool is_logged) {
	if (nick.empty()) {
		std::string err = ":localhost 431 * :No nickname given\r\n";
		send(c->getFd(), err.c_str(), err.length(), 0);
		return;
	}

	std::vector<std::string> &used_nicks = this->_server.getUsedNicks();
	std::vector<std::string>::iterator it = std::find(used_nicks.begin(), used_nicks.end(), nick[0]);

	if (it != used_nicks.end()) {
		std::string msg = ":localhost 433 " + c->getNickname() + " " + nick[0] + " :Nickname is already in use\r\n";
		send(c->getFd(), msg.c_str(), msg.length(), 0);
	}
	else {
		std::string old_nick = c->getNickname();
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
			std::string welcome = ":localhost 001 " + c->getNickname() + " :Welcome to the IRC Network\r\n";
			send(c->getFd(), welcome.c_str(), welcome.length(), 0);
		}
		else {
			c->setStatus(NICKNAME);
		}
	}
}

void Command::cmd_user(std::vector<std::string> arg, Client *c) {
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
		std::string welcome = ":localhost 001 " + c->getNickname() + " :Welcome to the IRC Network\r\n";
		send(c->getFd(), welcome.c_str(), welcome.length(), 0);
	} else {
		c->setStatus(USERNAME);
	}
}

void Command::cmd_ping(std::vector<std::string> arg, Client *c) {
	if (arg.empty())
		return;
	std::string msg = ":localhost PONG * :" + arg[0] + "\r\n";
	send(c->getFd(), msg.c_str(), msg.length(), 0);
}

// void Command::cmd_join(std::vector<std::string> arg, Client *c) {
// 	if()
// }