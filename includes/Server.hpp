/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 02:53:49 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/18 09:21:58 by mcomin           ###   ########.fr       */
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

