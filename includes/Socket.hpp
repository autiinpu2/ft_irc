/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:34:53 by mathys            #+#    #+#             */
/*   Updated: 2026/09/25 05:59:41 by mathys           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
 
class Socket {
	private:
		int _fd;
	public:
		Socket(int port);
		~Socket();
 
		int getFd(void) const;
		int acceptClient(void) const;
};
 