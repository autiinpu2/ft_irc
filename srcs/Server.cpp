/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:40 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/22 06:29:13 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.cpp"
#include "Client.hpp"

Server* Server::_instance = NULL;

Server::Server(long port, const std::string &password) :  nb_clients(0), serv_port(port), serv_password(password) {
	struct sockaddr_in sock;
	struct in_addr addr;

	sock.sin_port = htons(Server::serv_port);
	sock.sin_family = AF_INET;
	if (!inet_aton("0.0.0.0", &addr))
		throw std::runtime_error("bad init of in_addr");
	sock.sin_addr = addr;
	Server::serv_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Server::serv_socket < 0)
		throw std::runtime_error("socket init failed");
	int opt = 1;
    if (setsockopt(Server::serv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("setsockopt failed");
	
	if (fcntl(Server::serv_socket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl failed on server fd");
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

int Server::getNbClient(void) const {
	return Server::nb_clients;
}

void	Server::setNbClient(int nb){
	Server::nb_clients = Server::nb_clients + nb;
}

void Server::handle_tokens(const std::string &buffer, Client *c) {
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
		
		lineStream >> arg;
		if (command == "PASS")
			cmd_pass(arg, c);
		else if (command == "PING")
			cmd_ping(arg, c);
	}
}

void Server::cmd_pass(std::string pass, Client *c) {
	if (pass == Server::serv_password) {
		c->setStatus(true);
	}
	else {
		char msg[45] = "::localhost 464 * :Password incorrect\r\n";
		send(c->getFd(), msg, 45, 0);
	}
}

void Server::cmd_ping(std::string arg, Client *c) {
	std::string msg = ":localhost PONG * :" + arg + "\r\n";
	
	send(c->getFd(), msg.c_str(), strlen(msg.c_str()), 0);
}

fd_set Server::init_rfds(std::vector<Client*> clients) {
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(Server::serv_socket, &rfds);
	for (size_t i = 0; i < clients.size(); ++i)
		FD_SET(clients[i]->getFd(), &rfds);
	return rfds;
}

int	Server::init_client(fd_set &rfds, std::vector<Client*> &clients) {
	if (FD_ISSET(Server::serv_socket, &rfds)) {
		try {
			Client *c = new Client();
			if (c->getFd() >= 0) {
				// FD_SET(c->getFd(), &rfds);
				clients.push_back(c);
				std::cout << "\033[1;32mNew client connected! (FD: " << c->getFd() << ")" << std::endl;
				Server::nb_clients++;
				return 0;
			}
			return -1;
		} 
		catch (std::runtime_error &e){
			std::cerr << e.what() << std::endl;
			return -1;
		}
	}
	return 0;
}

void	Server::init_buffer(fd_set &rfds, std::vector<Client*> &clients) {
	for (size_t i = 0; i < clients.size(); ++i) {			
		if (clients[i]->getFd() > 0 && FD_ISSET(clients[i]->getFd(), &rfds)) {
			char buffer[1024];
			buffer[0] = 't';
			int bytes_read = recv(clients[i]->getFd(), buffer, 1023, 0);
			
			if (bytes_read <= 0) {
				Server::getInstance().setNbClient(-1);
				std::cout << "\033[1;31mClient disconnected. (FD: " << clients[i]->getFd() << ") Remaining: " << Server::getInstance().getNbClient() << "\033[1;37m" << std::endl;
				delete clients[i];
				clients.erase(clients.begin() + i);
				--i;
			}
			else {
				buffer[bytes_read] = '\0';
				Server::getInstance().handle_tokens(buffer, clients[i]);
				std::cout << "\033[1;33mReceived: " << buffer << "\033[1;37m" << std::endl;
				bzero(buffer, 1024);
			}
		}
	}
}

int Server::serv_loop(void) {
	std::vector<Client*> clients;
	signal(SIGINT, handle_signal);

	std::cout << "\033[1;92mServer ON\n\033[m" << std::endl;
	while (true) {
		if (signalstatus == SIGINT)
			break;
		
		fd_set rfds = init_rfds(clients);
		
		int ret = select(max_fd(clients)+ 1, &rfds, NULL, NULL, NULL);
		if (ret == -1) {
			if (errno == EINTR)
				break;
			return -1;
		}

		if (init_client(rfds, clients) == -1)
			return -1;
		init_buffer(rfds, clients); 
	}
	for (size_t i = 0; i < clients.size(); ++i) {
		delete clients[i];
	}
	std::cout << "\033[1;31m\nServer OFF\033[1;31m" << std::endl;
	return 0;
}
