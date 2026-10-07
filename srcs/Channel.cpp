/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 03:25:15 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/08 00:33:14 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

# include <sys/socket.h>

Channel::Channel(const std::string &n_channel, Client *c) : _name(n_channel) {
	this->_clients.insert(c);
	this->_operator = c;
}

Channel::~Channel() {}

const std::string &Channel::getName(void) const {
	return this->_name;
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


void	Channel::broadcast(std::string msg, Client *sender)
{
	std::set<Client*>::iterator it1;
	bool is_in_chan = false;
	for (it1 = this->_clients.begin(); it1 != this->_clients.end(); ++it1) {
		Client* member = *it1;
		if (member == sender)
		{
			is_in_chan = true;
			break;
		}
	}
	if (is_in_chan == false) {
		std::string send_msg = ":localhost 404 " + sender->getNick() + " " + this->getName() + " :Cannot send to channel\r\n";
		send(sender->getFd(), send_msg.c_str(), send_msg.length(), 0);
		return;
	}
	std::set<Client*>::iterator it2;
	for (it2 = this->_clients.begin(); it2 != this->_clients.end(); ++it2) {
		Client* member = *it2;
		if (member != sender)
		{
			std::string send_msg = ":" + sender->getNick() + " PRIVMSG " + this->getName() + " :" + msg + "\r\n";
            send(member->getFd(), send_msg.c_str(), send_msg.length(), 0);
		}
	}
}
