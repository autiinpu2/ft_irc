/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:20:13 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/23 00:48:18 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>

enum LOG_STATUS {
	NONE,
	PASSWORD,
	NICKNAME,
	USERNAME,
	FULL
};

class Client {
	private:
		int fd;
		std::string _nickname;
		std::string _username;
		LOG_STATUS islog;
	public:
		Client(void);
		~Client();
	
	int getFd(void) const;
	void setStatus(LOG_STATUS status);
	void setNickname(std::string nick);
	void setUsername(std::string user);
	LOG_STATUS getStatus(void) const;
	std::string getNickname(void) const;
	std::string getUsername(void) const;
};
