/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:35:36 by mcomin            #+#    #+#             */
/*   Updated: 2026/11/04 03:33:20 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"
# include "Server.hpp"
# include "Client.hpp"

# include <sys/socket.h>
# include <sstream>
# include <iomanip>
# include <string>

int max_fd(std::vector<Client*> &clients) {
	int max = Server::getInstance().getSocket();

	for (size_t i = 0; i < clients.size(); ++i) {
		if (clients[i]->getFd() > max)
			max = clients[i]->getFd();
	}
	return max;
}

bool isDigits(const std::string &s) {
	return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
}

void err_msg(int err_code, std::string msg, Client *c)
{
	std::ostringstream oss;
	
	oss << ":ircserv " << std::setfill('0') << std::setw(3) << err_code << " ";
	
	std::string nick = c->getNick();
	if (nick.empty())
		nick = "*";

	oss << nick << " " << msg << "\r\n";

	std::string send_msg = oss.str();
	send(c->getFd(), send_msg.c_str(), send_msg.length(), 0);
}


size_t stringToSizeT(const std::string &str) {
	std::stringstream   ss(str);
	size_t              result;

	if (!(ss >> result)) {
		return 0; 
	}
	return result;
}

std::string lowerstring(std::string s) {
    std::string lowstring;
    for (int i = 0; s[i] != '\0'; i++) {
        lowstring += static_cast<char>(std::tolower(static_cast<unsigned char>(s[i])));
    }
    return lowstring;
}
