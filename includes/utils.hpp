/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuyane <apuyane@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 05:48:28 by mathys            #+#    #+#             */
/*   Updated: 2026/11/04 03:27:09 by apuyane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
# include <vector>
# include <string>

class Client;
 

bool isDigits(const std::string &s);
int max_fd(std::vector<Client*> &clients);
void err_msg(int err_code, std::string msg, Client *c);
size_t stringToSizeT(const std::string &str);
std::string lowerstring(std::string s);