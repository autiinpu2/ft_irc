/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 23:20:13 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/22 01:38:11 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Server.hpp"

class Client {
	private:
		int fd;
		bool authenticated;
	public:
		Client(void);
		~Client();
	
	int getFd(void) const;
	void setStatus(void);
};
