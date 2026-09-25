/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:58:44 by mathys            #+#    #+#             */
/*   Updated: 2026/09/25 05:59:26 by mathys           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
 
Parsing::Parsing(int ac, char **av) : _password(ac == 3 ? av[2] : "") {
	if (ac != 3)
		throw std::runtime_error("Error: wrong number of args");
 
	std::string port_str = av[1];
 
	size_t pos;
	int port;
	try {
		port = std::stoi(port_str, &pos);
	}
	catch (const std::exception &e) {
		throw std::runtime_error("Error: invalid port");
	}
 
	if (pos != port_str.size() || port < 6667 || 6669 < port)
		throw std::runtime_error("Error: invalid port");
 
	this->_port = port;
}
 
Parsing::~Parsing() {}
 