/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:23:46 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/22 01:30:49 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client(void) {
	this->fd = accept(Server::getInstance().getSocket(), NULL, NULL);
	if (this->fd < 0)
		throw std::runtime_error("fd accept failed");
	this->authenticated = false;
}

Client::~Client() {
	close(this->fd);
	delete this;
}

int Client::getFd(void) const {
	return this->fd;
}