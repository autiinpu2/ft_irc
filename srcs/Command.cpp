/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:51:32 by mathys            #+#    #+#             */
/*   Updated: 2026/10/07 03:56:10 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

# include <algorithm>
# include <sstream>
# include <sys/socket.h>

static void sendMsg(Client *c, const std::string &msg) {
	std::string m = msg + "\r\n";
	send(c->getFd(), m.c_str(), m.length(), 0);
}

static std::vector<std::string> splitArg(const std::string &s) {
	std::vector<std::string> 	res;
	std::stringstream 			ss(s);
	std::string 				arg;

	while (std::getline(ss, arg, ',')) {
		if (!arg.empty())
			res.push_back(arg);
	}
	return res;
}

Command::Command(Server &server) : _server(server) {}

Command::~Command() {}

static void parseLine(const std::string &line, std::string &command, std::vector<std::string> &arg) {
	std::stringstream 	ss(line);
	std::string 		token;

	ss >> command;
	while (ss >> token) {
		if (token[0] == ':') {
			std::string rest;
			std::getline(ss, rest);
			arg.push_back(token.substr(1) + rest);
			break;
		}
		arg.push_back(token);
	}
}

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
		parseLine(line, command, arg);

		LOG_STATUS status= c->getStatus();
		if (command == "PASS" && status == NONE)
			this->cmdPass(arg, c);
		else if (command == "NICK" && (status == PASSWORD || status == USERNAME))
			this->cmdNick(arg, c, false);
		else if (command == "USER" && (status == PASSWORD || status == NICKNAME))
			this->cmdUser(arg, c);
		else if (status != FULL)
			sendMsg(c, ":localhost 451 * :You have not registered");
		else if (command == "PING")
			this->cmdPing(arg, c);
		else if (command == "NICK")
			this->cmdNick(arg, c, true);
		else if (command == "USER")
			this->cmdUser(arg, c);
		else if (command == "JOIN")
			this->cmdJoin(arg, c);
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

	if (name.size() < 2 || name[0] != '#') {
		sendMsg(c, ":ircserv 403 " + c->getNick() + " " + name + " :No such channel");
		return;
	}
	if (it == channels.end()) {
		Channel *newChannel = new Channel(name, c);
		this->_server.addChannel(name, newChannel);
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
	channel->returnJOIN(c);
}

void Command::cmdJoin(std::vector<std::string> arg, Client *c) {
	if (arg.empty() || splitArg(arg[0]).empty()) {
		sendMsg(c, ":ircserv 461 " + c->getNick() + " JOIN :Not enough parameters");
		return;
	}

	std::vector<std::string> names = splitArg(arg[0]);
	std::vector<std::string> keys;
	if (arg.size() > 1)
		keys = splitArg(arg[1]);

	for (size_t i = 0; i < names.size(); ++i) {
		if (i < keys.size())
			this->joinChannel(names[i], keys[i], c);
		else
			this->joinChannel(names[i], "", c);
	}
}