/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:58:44 by mathys            #+#    #+#             */
/*   Updated: 2026/10/07 23:03:52 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "utils.hpp"
#include <sstream>

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

void Parsing::parseLine(const std::string &line, std::string &command, std::vector<std::string> &arg) {
	std::stringstream 	ss(line);
	std::string 		token;

	ss >> command;
	while (ss >> token) {
		if (token[0] == ':') {
			std::string rest;
			std::getline(ss, rest);
			arg.push_back(token.substr(1) + rest);
			break;
		}
		arg.push_back(token);
	}
}

std::vector<std::string> Parsing::splitArg(const std::string &s) {
	std::vector<std::string> res;
	std::string              item;
	size_t                   start = 0;
	size_t                   pos;

	while ((pos = s.find(',', start)) != std::string::npos) {
		res.push_back(s.substr(start, pos - start));
		start = pos + 1;
	}
	res.push_back(s.substr(start));
	return res;
}

bool Parsing::vectorEmpty(const std::vector<std::string> &v) {
	for (size_t i = 0; i < v.size(); ++i)
		if (v[i].empty())
			return true;
	return false;
}