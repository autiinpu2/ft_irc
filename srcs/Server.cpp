/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:40 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/25 06:28:15 by mathys           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Socket.hpp"
#include "Command.hpp"
#include "Client.hpp"
#include "utils.hpp"

# include <cerrno>
# include <sys/socket.h>

Server* Server::_instance = NULL;

Server::Server(int port, const std::string &password) : _nb_clients(0), _port(port), _password(password) {
	this->_socket = new Socket(this->_port);
	this->_command = new Command(*this);
}

Server::~Server() {
	for (size_t i = 0; i < this->_clients.size(); ++i)
		delete this->_clients[i];
	delete this->_socket;
	delete this->_command;
}

Server &Server::getInstance(long port, const std::string &password) {
	if (!_instance)
		_instance = new Server(static_cast<int>(port), password);
	return *_instance;
}

void Server::destroyInstance(void) {
	delete _instance;
	_instance = NULL;
}

const std::string &Server::getPassword(void) const {
	return this->_password;
}

long Server::getPort(void) const {
	return this->_port;
}

int Server::getSocket(void) const {
	return this->_socket->getFd();
}

int Server::getNbClient(void) const {
	return this->_nb_clients;
}

std::vector<std::string> &Server::getUsedNicks(void) {
	return this->_used_nicks;
}

void Server::setNbClient(int nb) {
	this->_nb_clients = this->_nb_clients + nb;
}

fd_set Server::initFd(void) const {
	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(this->getSocket(), &rfds);
	for (size_t i = 0; i < this->_clients.size(); ++i)
		FD_SET(this->_clients[i]->getFd(), &rfds);
	return rfds;
}

void Server::acceptNewClient(fd_set &rfds) {
	if (!FD_ISSET(this->getSocket(), &rfds))
		return;

	int fd = this->_socket->acceptClient();
	if (fd < 0)
		return ;

	try {
		Client *c = new Client(fd);
		this->_clients.push_back(c);
		this->setNbClient(1);
		std::cout << "\033[1;32mNew client connected! (FD: " << fd << ")\033[0m" << std::endl;
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		close(fd);
	}
}

void Server::handleMsg(fd_set &rfds) {
	for (size_t i = 0; i < this->_clients.size(); ++i) {
		Client *c = this->_clients[i];

		if (c->getFd() <= 0 || !FD_ISSET(c->getFd(), &rfds))
			continue;

		char buffer[1024];
		int bytes_read = recv(c->getFd(), buffer, sizeof(buffer) - 1, 0);

		if (bytes_read <= 0) {
			this->setNbClient(-1);
			std::cout << "\033[1;31mClient disconnected. (FD: " << c->getFd() << ") Remaining: " << this->getNbClient() << "\033[0m" << std::endl;
			delete c;
			this->_clients.erase(this->_clients.begin() + i);
			--i;
			continue;
		}

		buffer[bytes_read] = '\0';
		std::cout << "\033[1;33mReceived: " << buffer << "\033[0m" << std::endl;
		this->_command->handleCmd(buffer, c);
	}
}

int Server::serv_loop(void) {
	signal(SIGINT, handle_signal);

	std::cout << "\033[1;92mServer ON\n\033[0m" << std::endl;
	while (signalstatus != SIGINT) {
		fd_set rfds = this->initFd();

		int ret = select(max_fd(this->_clients) + 1, &rfds, NULL, NULL, NULL);
		if (ret == -1) {
			if (errno == EINTR)
				break;
			return -1;
		}

		this->acceptNewClient(rfds);
		this->handleMsg(rfds);
	}
	std::cout << "\033[1;31m\nServer OFF\033[0m" << std::endl;
	return 0;
}