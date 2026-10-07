/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 02:15:39 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/08 00:03:12 by mcomin           ###   ########.fr       */
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
		std::set<Client*>	_operators;
		std::string			_mode;
		std::string			_topic;
		std::string			_key;
		size_t				_limit;
		bool				_invite_only;
		std::set<Client*>	_invited;
	public:
		Channel(const std::string &n_channel, Client *c, const std::string key);
		~Channel();
 
		const std::string	&getName(void) const;
		const std::string	&getMode(void) const;
		std::string			getClients(void) const;
		const std::string	&getTopic(void) const;
		const std::string	&getKey(void) const;
		size_t				getLimit(void) const;
		size_t				getSize(void) const;
		
		bool				isOperator(Client *c) const;
		bool				isInviteOnly(void) const;
		bool				isInvited(Client *c) const;
		
		void				setKey(const std::string &key);
		void				setLimit(size_t limit);
		void				setInviteOnly(bool value);
		
		void				invite(Client *c);
		void				uninvite(Client *c);
		void				addClient(Client *c);
		void				removeClient(Client *c);
		bool				inChannel(Client *c) const;
		bool				isEmpty(void) const;
		
		void				reply(Client *c, const std::string &msg);
		void				replyALL(const std::string &msg);
		void				returnJOIN(Client *c);
};