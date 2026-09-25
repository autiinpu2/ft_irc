/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:37:32 by mathys            #+#    #+#             */
/*   Updated: 2026/09/25 06:27:09 by mathys           ###   ########.fr       */
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
 
		void	cmd_join(std::vector<std::string> arg, Client *c);
		void	cmd_pass(std::vector<std::string> arg, Client *c);
		void	cmd_nick(std::vector<std::string> nick, Client *c, bool is_logged);
		void	cmd_user(std::vector<std::string> pass, Client *c);
		void	cmd_ping(std::vector<std::string> arg, Client *c);
	public:
		Command(Server &server);
		~Command();
 
		void	handleCmd(const std::string &buffer, Client *c);
};
 