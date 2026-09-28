/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:37:32 by mathys            #+#    #+#             */
/*   Updated: 2026/09/28 06:57:51 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
# include <string>
# include <vector>
 
class Client;
class Server;
 
class Command {
	private:
		Server &_server;
 
		void	cmdJoin(std::vector<std::string> arg, Client *c);
		void	cmdPass(std::vector<std::string> arg, Client *c);
		void	cmdNick(std::vector<std::string> nick, Client *c, bool is_logged);
		void	cmdUser(std::vector<std::string> pass, Client *c);
		void	cmdPing(std::vector<std::string> arg, Client *c);
	public:
		Command(Server &server);
		~Command();
 
		void	handleCmd(const std::string &buffer, Client *c);
};
 