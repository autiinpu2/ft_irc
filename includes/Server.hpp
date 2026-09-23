/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:49 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/23 04:01:19 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

# include <cstdlib>
# include <ctime>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <cstring>
# include <exception>
# include <vector>
# include <csignal>
# include <string>
# include <sstream>
# include <fcntl.h>
# include <map>

# include "signal.hpp"

class Client;

class Server {
	private:
		static	Server* _instance;
		Server(long port, const std::string &password);
		
		int		serv_socket;
		int		nb_clients;
		long	serv_port; 
		const	std::string serv_password;
		std::map<std::string, Client*> channels;
		
	public:
		~Server();
		static	Server	&getInstance(long port = 0, const std::string &password = "");
	   	static	void	destroyInstance(void);
		
		int		serv_loop(void);
		void	handle_tokens(const std::string &buffer, Client *c);
		   
		const	std::string &getPassword(void) const;
		long	getPort(void) const;
		int		getSocket(void) const;
		int		getNbClient(void) const;
		void	setNbClient(int nb);

		void	cmd_pass(std::string pass, Client *c);
		void	cmd_ping(std::string arg, Client *c);
		void	cmd_join(std::string arg, Client *c);
		
		fd_set	init_rfds(std::vector<Client*> &clients);
		int		init_client(fd_set &rfds, std::vector<Client*> &clients);
		void	init_buffer(fd_set &rfds, std::vector<Client*> &clients);
};
