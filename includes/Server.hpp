/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:49 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/06 00:31:40 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <exception>
# include <sys/select.h>

# include "signal.hpp"

class	Client;
class	Channel;
class	Command;
class	Socket;

class Server {
	private:
		Server(int port, const std::string &password);

		static	Server 					*_instance;
		int								_nb_clients;
		int								_port;
		const std::string				_password;

		Socket							*_socket;
		Command							*_command;

		std::vector<Client*>			_clients;
		std::map<std::string, Channel*>	_channels;
		std::vector<std::string> 		_used_nicks;

		fd_set	initFd(void) const;
		void	acceptNewClient(fd_set &rfds);
		void	handleMsg(fd_set &rfds);

	public:
		~Server();
		static	Server	&getInstance(long port = 0, const std::string &password = "");
	   	static	void	destroyInstance(void);

		int				serv_loop(void);

		const			std::string &getPassword(void) const;
		long			getPort(void) const;
		int				getSocket(void) const;
		int				getNbClient(void) const;
		std::vector<std::string> &getUsedNicks(void);
		const std::map<std::string, Channel*> &getChannel(void) const;
		std::vector<Client*> &getClients (void);
		void			addChannel(const std::string &n_channel, Channel *ch);

		void			setNbClient(int nb);
};