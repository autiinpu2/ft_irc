/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:49 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/21 22:11:26 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
# include <cstdlib>
# include <ctime>
#include <sstream>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <cstring>
# include <exception>
# include <vector>
# include <map>

class Server {
	private:
		static Server* _instance;
		Server(long port, const std::string &password);
		
		int _fd;
		int _nbc;
		long _port; 
		const std::string _password;
	public:
		~Server();
		static Server &getInstance(long port = 0, const std::string &password = "");
	   	static void	destroyInstance(void);
		long getPort(void) const;
		const std::string &getPassword(void) const;
		int serv_loop(void);
		
};

