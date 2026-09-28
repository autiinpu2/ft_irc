/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:58:44 by mathys            #+#    #+#             */
/*   Updated: 2026/09/28 05:47:49 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "utils.hpp"

Parsing::Parsing(int ac, char **av) : _password(ac == 3 ? av[2] : "") {
	if (ac != 3)
		throw std::runtime_error("Error: wrong number of args");
 
	try {
		std::string portStr = av[1];
		int port = checkPort(portStr);
		this->_port = port;
	}
	catch(std::runtime_error &e) {
		throw std::runtime_error(e.what()); 
	}
}
  
Parsing::~Parsing() {}
 
int Parsing::checkPort(const std::string &s) {
	if (s.empty() || s.size() > 5)
		throw std::runtime_error("Error: invalid port");
	if (s.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("Error: invalid port");
	long port = std::strtol(s.c_str(), NULL, 10);
	if ( port < 6665 || 6669 < port)
		throw std::runtime_error("Error: invalid port");
	return port;
}