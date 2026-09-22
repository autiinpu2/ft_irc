/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:40 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/22 01:37:26 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "utils.cpp"

Server* Server::_instance = NULL;

Server::Server(long port, const std::string &password) :  nb_clients(0), serv_port(port), serv_password(password) {
	struct sockaddr_in sock;
	struct in_addr addr;

	(void)Server::nb_clients;
	sock.sin_port = htons(Server::serv_port);
	sock.sin_family = AF_INET;
	if (!inet_aton("0.0.0.0", &addr))
		throw std::runtime_error("bad init of in_addr");
	sock.sin_addr = addr;
	Server::serv_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Server::serv_socket < 0)
			throw std::runtime_error("socket init failed");
	if (bind(Server::serv_socket, (struct sockaddr *)&sock, sizeof(sockaddr_in)))
			throw std::runtime_error("bind failed");
	if (listen(Server::serv_socket, 10) == -1)
			throw std::runtime_error("listening fd failed");
}

Server::~Server() {
	close(Server::serv_socket);
}; 

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

const std::string &Server::getPassword(void) const {
	return Server::serv_password;
}

long Server::getPort() const {
	return Server::serv_port;
}

int Server::getSocket(void) const {
	return Server::serv_socket;
}

void Server::handle_tokens(const std::string &buffer, int fd_client) {
	std::stringstream stream(buffer);
	std::string line;
	std::string arg;

	while (std::getline(stream, line)) {
		if (line.empty())
			continue;
		if (line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);
			
		std::stringstream lineStream(line);
		std::string command;
		lineStream >> command;
		
		std::cout << "Commande IRC : [" << command << "]" << std::endl;
		lineStream >> arg;
		if (command == "PASS")
			cmd_PASS(arg, fd_client);
	}
}

void Server::cmd_PASS(std::string pass, Client c) {
	if (pass == Server::serv_password) {
		c.authenticated = false;
		std::cout << "Password good" << std::endl;
	}
	else {
		std::cout << "Password not good" << std::endl;
		char msg[45] = ":serveur.irc.com 464 * :Password incorrect\r\n";
		send(fd_client, msg, 45, 0);
		close(fd_client);
	}
}

int Server::serv_loop(void) {
	std::vector<Client> clients;
	signal(SIGINT, handle_signal);
	

	std::cout << "\033[1;32mServer ON\n\033[1;32m" << std::endl;
	while (true) {
		if (signalstatus == SIGINT)
			break;
		
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(Server::serv_socket, &rfds);
		for (size_t i = 0; i < clients.size(); ++i)
			FD_SET(clients[i].getFd(), &rfds);
		
		int ret = select(max_fd(clients)+ 1, &rfds, NULL, NULL, NULL);
		if (ret == -1) {
			if (errno == EINTR)
				break;
			return 1;
		}

		if (FD_ISSET(Server::serv_socket, &rfds)) {
			try {
				Client *c = new Client();
				if (c->getFd() >= 0) {
					FD_SET(c->getFd(), &rfds);
					clients.push_back(*c);
					std::cout << "\033[1;32mNew client connected! (FD: " << c->getFd() << ")" << std::endl;
					Server::nb_clients++;
				}
			} 
			catch (std::runtime_error &e){
				std::cerr << e.what() << std::endl;
				return 1;
			}
		}
		
		for (size_t i = 0; i < clients.size(); ++i) {			
			if (clients[i].getFd() > 0 && FD_ISSET(clients[i].getFd(), &rfds)) {
				char buffer[1024];
				int bytes_read = recv(clients[i].getFd(), buffer, 1023, 0); 
				
				if (bytes_read <= 0) {
					Server::nb_clients--;
					std::cout << "\033[1;31mClient disconnected. (FD: " << clients[i].getFd() << ") Remaining: " << Server::nb_clients << "\033[1;37m" << std::endl;
					clients.erase(clients.begin() + i);
					--i;
				}
				else {
					buffer[bytes_read] = '\0';
					handle_tokens(buffer, clients[i]);
					std::cout << "\033[1;33mReceived: " << buffer << "\033[1;37m" << std::endl;
					send(clients[i].getFd(), buffer, bytes_read, 0);
					bzero(buffer, 1024);
				}
			}
		}
	}
	std::cout << "\033[1;31m\nServer OFF\033[1;31m" << std::endl;
	return 0;
}
