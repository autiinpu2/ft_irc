/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 03:25:15 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/25 06:11:54 by mathys           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::vector<Client*>&c, std::string channel_n) : _channel_name(channel_n) {
	for (std::vector<Client*>::iterator it = c.begin(); it != c.end(); ++it) {
		_channel_clients.insert(*it);
	}
}

Channel::~Channel() {}

const std::string &Channel::getName(void) const {
	return this->_channel_name;
}

void Channel::addClient(Client *c) {
	this->_channel_clients.insert(c);
}

void Channel::removeClient(Client *c) {
	this->_channel_clients.erase(c);
}

bool Channel::inChannel(Client *c) const {
	return this->_channel_clients.find(c) != this->_channel_clients.end();
}

bool Channel::isEmpty(void) const {
	return this->_channel_clients.empty();
}