/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 05:20:22 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/25 05:48:51 by mathys           ###   ########.fr       */
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
	catch (std::exception &e) {
		std::cerr << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
		return 1;
	}
}
 