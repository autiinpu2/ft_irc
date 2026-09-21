/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 22:23:37 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/21 22:38:33 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <csignal>
# include <cerrno>

extern volatile sig_atomic_t signalstatus;

void handle_signal(int sig);