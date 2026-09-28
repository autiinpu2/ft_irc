/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:58:01 by mathys            #+#    #+#             */
/*   Updated: 2026/09/28 04:19:12 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <iostream>
# include <cstdlib>

class Parsing {
    public:
        Parsing(int ac, char **av);
        ~Parsing();

        long                 _port;
        const std::string   _password;

        int checkPort(const std::string &s);
};