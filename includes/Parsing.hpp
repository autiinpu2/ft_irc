/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathys <mathys@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 03:58:01 by mathys            #+#    #+#             */
/*   Updated: 2026/09/25 06:19:33 by mathys           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <iostream>

class Parsing {
    public:
        Parsing(int ac, char **av);
        //Parsing(std::string line);
        ~Parsing();

        int                 _port;
        const std::string   _password;
};