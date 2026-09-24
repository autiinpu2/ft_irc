/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 03:25:15 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/24 06:55:35 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::vector<Client*>&c, std::string n_channel) : name(n_channel), topic("NEW CHANNEL\n") {
	for (std::vector<Client*>::iterator it = c.begin(); it != c.end(); ++it) {
		clients.insert(*it);
	}
}

Channel::Channel(Client* c, std::string n_channel) : name(n_channel),  topic("NEW CHANNEL\n") {
	clients.insert(c);
}

Channel::~Channel() {}

void Channel::printClients(void) const {
	for (std::set<Client*>::const_iterator it; it != clients.end(); ++it) {
		std::cout << *it << std::endl;
	}
}

std::string Channel::getTopic(void) const {
	return this->topic;
}

std::string Channel::getName(void) const {
	return this->name;
}

void	Channel::addclient(Client *c) {
	clients.insert(c);
}