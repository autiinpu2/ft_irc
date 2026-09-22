/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:35:36 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/22 03:23:05 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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