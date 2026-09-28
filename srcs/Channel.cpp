/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 03:25:15 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/28 05:33:24 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

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