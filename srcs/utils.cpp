/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:35:36 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/28 04:03:25 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Server.hpp"
#include "Client.hpp"
 
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


 