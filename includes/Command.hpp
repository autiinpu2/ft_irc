/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:37:32 by mathys            #+#    #+#             */
/*   Updated: 2026/10/08 00:43:46 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
# include <string>
# include <vector>

class Client;
class Server;
struct ChannelMode;

enum MODE {
	ADD,
	REMOVE
};

class Channel;

class Command {
	private:
		Server &_server;
 
		void	cmdJoin(std::vector<std::string> arg, Client *c);
		void	joinChannel(const std::string &name, const std::string &key, Client *c);
		
		void	cmdPass(std::vector<std::string> arg, Client *c);
		
		void	cmdNick(std::vector<std::string> nick, Client *c, bool is_logged);
		
		void	cmdUser(std::vector<std::string> pass, Client *c);
		
		void	cmdPing(std::vector<std::string> arg, Client *c);
		void	cmdMsg(std::vector<std::string> arg, Client *c);
		
		void	cmdInvite(std::vector<std::string> arg, Client *c);
		void	cmdTopic(std::vector<std::string> arg, Client *c);
		//Mode
		void	cmdMode(std::vector<std::string> arg, Client *c);
		void 	cmdModeI(ChannelMode mode, Channel *chan, Client *c);
		void 	cmdModeL(ChannelMode mode, Channel *chan, Client *c);
		void 	cmdModeT(ChannelMode mode, Channel *chan, Client *c);
		void	cmdModeO(ChannelMode mode, Channel *chan, Client *c);
		void	cmdModeK(ChannelMode mode, Channel *chan, Client *c);

	public:
		Command(Server &server);
		~Command();
 
		void	handleCmd(const std::string &buffer, Client *c);
};