/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:51:58 by mathys            #+#    #+#             */
/*   Updated: 2026/09/25 06:00:00 by mathys           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

# include <stdexcept>
# include <cstring>

Socket::Socket(int port) : _fd(-1) {
	struct sockaddr_in sock;
	struct in_addr addr;

	std::memset(&sock, 0, sizeof(sock));
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;

	if (!inet_aton("0.0.0.0", &addr))
		throw std::runtime_error("bad init of in_addr");

	sock.sin_addr = addr;

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
		throw std::runtime_error("socket init failed");

	int opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt failed");
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl failed on server fd");
	if (bind(this->_fd, (struct sockaddr *)&sock, sizeof(sockaddr_in)) < 0)
		throw std::runtime_error("bind failed");
	if (listen(this->_fd, 10) == -1)
		throw std::runtime_error("listening fd failed");
}

Socket::~Socket() {
	if (this->_fd >= 0)
		close(this->_fd);
}

int Socket::getFd(void) const {
	return this->_fd;
}

int Socket::acceptClient(void) const {
	return accept(this->_fd, NULL, NULL);
}