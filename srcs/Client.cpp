/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:23:46 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/06 01:06:59 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _status(NONE) {
	this->_nickname = "*";
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl failed on client fd");
}

Client::~Client() {
	close(this->_fd);
}

int Client::getFd(void) const {
	return this->_fd;
}

void Client::setStatus(LOG_STATUS status) {
	this->_status = status;
}

LOG_STATUS Client::getStatus(void) const {
	return this->_status;
}

void Client::setNickname(std::string nick) {
	this->_nickname = nick;
}

void Client::setUsername(std::string user) {
	this->_username = user;
}

void Client::setRealname(std::string name) {
	this->_real_name = name;
}

const std::string &Client::getNick(void) const {
	return this->_nickname;
}

std::string Client::getUser(void) const {
	return this->_username;
}

std::string Client::getRealname(void) const {
	return this->_real_name;
}