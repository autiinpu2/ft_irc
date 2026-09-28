/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 05:48:28 by mathys            #+#    #+#             */
/*   Updated: 2026/09/28 05:37:31 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
# include <vector>
# include <string>

class Client;
 

bool isDigits(const std::string &s);
int max_fd(std::vector<Client*> &clients);
 