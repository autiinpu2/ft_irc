/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 02:15:39 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/23 06:13:21 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

# include <iostream>
# include <vector>
# include <set>

class Client;

class Channel {
	private:
		std::string name;
		std::set<Client*> clients;
		std::string topic;
	public:
		Channel(std::vector<Client*>&c, std::string n_channel);
		Channel(Client *c, std::string n_channel);
		~Channel();

	void printClients(void) const;
	std::string getTopic(void) const;
	std::string getName(void) const;
};