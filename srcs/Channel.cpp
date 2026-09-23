/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 03:25:15 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/23 03:58:57 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::vector<Client*>&c, std::string n_channel) : name(n_channel) {
	for (std::vector<Client*>::iterator it = c.begin(); it != c.end(); ++it) {
		clients.insert(*it);
		std::cout << *it << std::endl;
	}
}

Channel::~Channel() {}