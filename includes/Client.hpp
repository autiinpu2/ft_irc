/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:20:13 by mcomin            #+#    #+#             */
/*   Updated: 2026/10/08 00:15:11 by mcomin           ###   ########.fr       */
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
};
