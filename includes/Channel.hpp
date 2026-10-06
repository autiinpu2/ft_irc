/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 02:15:39 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/06 01:36:57 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
# include <set>
# include <string>
# include <vector>
# include <sstream>
 
class Client;
 
class Channel {
	private:
		std::string			_name;
		std::set<Client*>	_clients;
		Client				*_operator;
		std::string			_mode;
		std::string			_topic;
	public:
		Channel(const std::string &n_channel, Client *c);
		~Channel();
 
		const std::string	&getName(void) const;
		const std::string	&getMode(void) const;
		const std::string	&getOperator(void) const;
		const std::string 	&getClients(void) const;
		void				addClient(Client *c);
		void				removeClient(Client *c);
		bool				inChannel(Client *c) const;
		bool				isEmpty(void) const;
		void				reply(Client *c, const std::string &msg);
		void				replyALL(const std::string &msg);
		void				returnJOIN(Client *c, std::string status);
};
 