/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:58:01 by mathys            #+#    #+#             */
/*   Updated: 2026/10/07 23:01:41 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <iostream>
# include <cstdlib>
# include <vector>

class Parsing {
    public:
        Parsing(int ac, char **av);
        ~Parsing();

        long                 _port;
        const std::string   _password;

        int                                 checkPort(const std::string &s);
        static void                         parseLine(const std::string &line, std::string &command, std::vector<std::string> &arg);
        static std::vector<std::string> splitArg(const std::string &s);
        static bool vectorEmpty(const std::vector<std::string> &v);
};