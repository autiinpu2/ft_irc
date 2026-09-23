/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:23:46 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/23 01:12:37 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client(void) {
	this->fd = accept(Server::getInstance().getSocket(), NULL, NULL);
	if (this->fd < 0)
		throw std::runtime_error("fd accept failed");
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl failed on client fd");
	this->islog = NONE;
}

Client::~Client(){
	close(this->fd);
}

int Client::getFd(void) const {
	return this->fd;
}

void  Client::setStatus(LOG_STATUS status) {
	this->islog = status;
}

LOG_STATUS  Client::getStatus(void) const {
	return this->islog;
}

void Client::setNickname(std::string nick) {
	this->_nickname = nick;
}

void Client::setUsername(std::string user) {
	this->_username = user;
}

std::string Client::getNickname(void) const {
	return this->_nickname;
}

std::string Client::getUsername(void) const {
	return this->_username;
}
