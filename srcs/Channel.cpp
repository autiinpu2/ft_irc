/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 03:25:15 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/06 01:58:07 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sys/socket.h>
#include "Client.hpp"

Channel::Channel(const std::string &n_channel, Client *c) : _name(n_channel), _topic("") {
	this->_clients.insert(c);
	this->_operator = c;
	this->_mode = "+nt";
}

Channel::~Channel() {}

const std::string &Channel::getName(void) const {
	return this->_name;
}

const std::string	&Channel::getMode(void) const {
	return this->_mode;		
}

const std::string	&Channel::getOperator(void) const {
	return this->_operator->getNick();		
}

const std::string &Channel::getClients(void) const {
	std::string clients;
	std::set<Client*>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		if (*it) {
			if (!clients.empty())
				clients += " ";
			if ((*it)->getNick() == this->getOperator())
				clients += "@";
			clients += (*it)->getNick();
		}
	}
	return clients;
}

void Channel::addClient(Client *c) {
	this->_clients.insert(c);
}

void Channel::removeClient(Client *c) {
	this->_clients.erase(c);
}

bool Channel::inChannel(Client *c) const {
	return this->_clients.find(c) != this->_clients.end();
}

bool Channel::isEmpty(void) const {
	return this->_clients.empty();
}

void Channel::reply(Client *c, const std::string &msg) {
	std::string m = msg + "\r\n";
	send(c->getFd() , m.c_str(), m.length(), 0);
}

void Channel::replyALL(const std::string &msg) {
	std::set<Client*>::const_iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		reply(*it, msg);
	}
}

void Channel::returnJOIN(Client *c, std::string status) {
	
	if (status == "new") {
		std::string prefix = ":" + c->getNick() + "!" + c->getUser();
		reply(c, prefix + " JOIN " + this->getName()); 
		reply(c, ":ircserv MODE " + this->getName() + this->getMode());
		reply(c, ":ircserv 353 " + c->getNick() + " = " + this->getName() + " :@" + this->getOperator());
		reply(c, ":ircserv 366 " + c->getNick() + " " + this->getName() + " :End of /NAMES list");
	}
	else if (status == "exists") {
		std::string prefix = ":" + c->getNick() + "!" + c->getUser();
		reply(c, prefix + " JOIN " + this->getName()); 
		reply(c, ":ircserv 332 " + c->getNick() + this->getName() + " :" + this->_topic);
		reply(c, ":ircserv 353 " + c->getNick() + " = " + this->getName() + this->getClients());
		reply(c, ":ircserv 366 " + c->getNick() + " = " + this->getName() + " :End of /NAMES list");
	}
}


