/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:20:13 by mcomin            #+#    #+#             */
/*   Updated: 2026/11/04 02:01:17 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <fcntl.h>
# include <unistd.h>

enum LOG_STATUS {
	NONE,
	PASSWORD,
	NICKNAME,
	USERNAME,
	FULL
};

class Client {
	private:
		std::string		buffer;
		int 			_fd;
		std::string 	_nickname;
		std::string 	_username;
		std::string 	_real_name;
		LOG_STATUS		_status;
		size_t			_nb_channels;
	public:
		Client(int fd);
		~Client();

	int 				getFd(void) const;
	const std::string 	&getNick(void) const;
	std::string 		getUser(void) const;
	std::string			getRealname(void) const;
	LOG_STATUS			getStatus(void) const;

	void 				setStatus(LOG_STATUS status);
	void 				setNickname(std::string nick);
	void 				setUsername(std::string user);
	void 				setRealname(std::string name);
	void				setNbChannels(size_t nb);

	std::string 		getbuffer();
	void				setBuffer(std::string buff);
};
