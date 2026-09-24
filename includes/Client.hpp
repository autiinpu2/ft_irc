/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:20:13 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/24 03:16:41 by mcomin           ###   ########.fr       */
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
		std::string _real_name;
		LOG_STATUS islog;
	public:
		Client(void);
		~Client();
	
	int getFd(void) const;
	void setStatus(LOG_STATUS status);
	void setNickname(std::string nick);
	void setUsername(std::string user);
	void setRealname(std::string name);
	
	LOG_STATUS getStatus(void) const;
	
	std::string getNickname(void) const;
	std::string getUsername(void) const;
	std::string getRealname(void) const;
};
