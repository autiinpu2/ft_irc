/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:23:46 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/22 04:28:11 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client(void) {
	this->fd = accept(Server::getInstance().getSocket(), NULL, NULL);
	if (this->fd < 0)
		throw std::runtime_error("fd accept failed");
	this->islog = false;
}

Client::~Client(){
	close(this->fd);
}

int Client::getFd(void) const {
	return this->fd;
}

void  Client::setStatus(bool status) {
	this->islog = status;
}

bool  Client::getStatus(void) const {
	return this->islog;
}