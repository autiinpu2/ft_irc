/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 05:20:22 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/21 22:21:40 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static long parsing(int ac, char **av) {
	char *endptr;
	
	if (ac != 3) {
		std::cerr << "\033[1;31mError: wrong number of agrs\033[1;37m" << std::endl;
		return -1;
	}
	long port = strtol(av[1], &endptr, 10);
	if (*endptr || (port < 0 || port > 65535)) {
		std::cerr << "\033[1;31mError: invalid port\033[1;31m" << std::endl;
		return -1;
	}
	return port;
}

int main(int ac, char **av) {
	long port = parsing(ac, av);
	if (port == -1)
		return 1;
	std::string password = av[2];
	try
	{
		Server &server = Server::getInstance(port, password);
		server.serv_loop();
		
	}
	catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	Server::destroyInstance();
	return 0;
}
