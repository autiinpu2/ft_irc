/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 03:25:15 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/08 00:01:20 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sys/socket.h>
#include "Client.hpp"

Channel::Channel(const std::string &n_channel, Client *c, const std::string key) : _name(n_channel), _topic(""), _key(key), _limit(0), _invite_only(false) {
	this->_clients.insert(c);
	this->_operators.insert(c);
	this->_mode = "+nt";
}

Channel::~Channel() {}

const std::string &Channel::getName(void) const {
	return this->_name;
}

const std::string	&Channel::getMode(void) const {
	return this->_mode;		
}

std::string Channel::getClients(void) const {
	std::string clients;
	std::set<Client*>::const_iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		if (*it) {
			if (!clients.empty())
				clients += " ";
			if (isOperator(*it) == true)
				clients += "@";
			clients += (*it)->getNick();
		}
	}
	return clients;
}

const std::string &Channel::getTopic(void) const {
	return this->_topic;
}


const std::string &Channel::getKey(void) const {
	return this->_key;
}

size_t Channel::getLimit(void) const {
	return this->_limit;
}

size_t Channel::getSize(void) const {
	return this->_clients.size();
}

bool	Channel::isOperator(Client *c) const {
	if (this->_operators.find(c) != this->_operators.end())
		return true;
	return false;
}

bool Channel::isInviteOnly(void) const {
	return this->_invite_only;
}

bool Channel::isInvited(Client *c) const {
	return this->_invited.find(c) != this->_invited.end();
}

void Channel::setKey(const std::string &key) {
	this->_key = key;
}

void Channel::setLimit(size_t limit) {
	this->_limit = limit;
}

void Channel::setInviteOnly(bool value) {
	this->_invite_only = value;
}

void Channel::invite(Client *c) {
	this->_invited.insert(c);
}

void Channel::uninvite(Client *c) {
	this->_invited.erase(c);
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

void Channel::returnJOIN(Client *c) {
	std::string nick = c->getNick();
	this->replyALL(":" + c->getNick() + "!" + c->getUser() + " JOIN " + this->_name);
	if (!this->_topic.empty())
		this->reply(c, ":ircserv 332 " + c->getNick() + " " + this->_name + " :" + this->_topic);
	this->reply(c, ":ircserv 353 " + c->getNick() + " = " + this->_name + " :" + this->getClients());
	this->reply(c, ":ircserv 366 " + c->getNick() + " " + this->_name + " :End of /NAMES list.");
}