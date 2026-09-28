/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 05:20:22 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/28 04:14:24 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parsing.hpp"
 
# include <iostream>
 
int main(int ac, char **av) {
	try {
		Parsing parsing(ac, av);
		Server &server = Server::getInstance(parsing._port, parsing._password);
		int ret = server.serv_loop();
		Server::destroyInstance();
		return ret;
	}
	catch (std::runtime_error &e) {
		std::cerr << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
		return 1;
	}
}
 