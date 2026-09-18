/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:40 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/18 09:52:41 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server* Server::_instance = NULL;

Server::Server(long port, const std::string &password) :  _nbc(0), _port(port), _password(password) {
	struct sockaddr_in sock;
	struct in_addr addr;

	(void)_nbc;
	sock.sin_port = htons(Server::_port);
	sock.sin_family = AF_INET;
	if (!inet_aton("0.0.0.0", &addr))
		throw std::runtime_error("bad init of in_addr");
	sock.sin_addr = addr;
	Server::_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (Server::_fd < 0)
			throw std::runtime_error("socket init failed");
	if (bind(Server::_fd, (struct sockaddr *)&sock, sizeof(sockaddr_in)))
			throw std::runtime_error("bind failed");
	if (listen(Server::_fd, 10) == -1)
			throw std::runtime_error("listening fd failed");
}

Server::~Server() {}; 

Server &Server::getInstance(long port, std::string const& password)
{
	if (!_instance)
		_instance = new Server(port, password);
	return *_instance;
}

void Server::destroyInstance(void) {
	delete _instance;
	_instance = NULL;
}

long Server::getPort() const {
	return Server::_port;
}

const std::string &Server::getPassword(void) const {
	return Server::_password;
}

int Server::serv_loop(void) {
	std::vector<int> client_fds;
	
	while (true) {
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(Server::_fd, &rfds);
		int max_fd = Server::_fd;

		for (size_t i = 0; i < client_fds.size(); ++i) {
			FD_SET(client_fds[i], &rfds);
			if (client_fds[i] > max_fd)
				max_fd = client_fds[i];
		}

		if (select(max_fd + 1, &rfds, NULL, NULL, NULL) == -1)
			return 1;

		if (FD_ISSET(Server::_fd, &rfds)) {
			int new_client = accept(Server::_fd, NULL, NULL);
			if (new_client >= 0) {
				client_fds.push_back(new_client);
				std::cout << "\033[1;32mNew client connected! (FD: " << new_client << ")" << std::endl;
				Server::_nbc++;
			}
		}
		for (size_t i = 0; i < client_fds.size(); ++i) {
			if (client_fds[i] > 0 && FD_ISSET(client_fds[i], &rfds)) {
				char buffer[1024];
				memset(buffer, 0, 1024);
				int bytes_read = recv(client_fds[i], buffer, 1023, 0);
				
				if (bytes_read <= 0) {
					client_fds.erase(client_fds.begin() + i);
					Server::_nbc--;
					std::cout << "\033[1;31mClient disconnected. (FD: " << client_fds[i] << ") Remaining: " << Server::_nbc << "\033[1;37m" << std::endl;
				} 
				else {
					std::cout << "\033[1;33mReceived: " << buffer << "\033[1;37m" << std::endl;
					send(client_fds[i], buffer, bytes_read, 0);
				}
			}
		}
	}
	return 0;
}
