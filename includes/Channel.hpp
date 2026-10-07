/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 02:15:39 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/06 03:10:01 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
# include <set>
# include <string>
# include <vector>

# include "Client.hpp"
 
class Client;
 
class Channel {
	private:
		std::string			_name;
		std::set<Client*>	_clients;
		Client				*_operator;
	public:
		Channel(const std::string &n_channel, Client *c);
		~Channel();
 
		const std::string	&getName(void) const;
		void				addClient(Client *c);
		void				removeClient(Client *c);
		bool				inChannel(Client *c) const;
		bool				isEmpty(void) const;
		void				broadcast(std::string msg, Client *sender);
};
