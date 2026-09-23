/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 02:15:39 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/23 03:37:41 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

# include "Server.hpp"
# include <set>

class Client;

class Channel {
	private:
		std::string name;
		std::set<Client*> clients;
	public:
		Channel(std::vector<Client*>&c, std::string n_channel);
		~Channel();
	
};