/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcomin <mcomin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 22:33:57 by mcomin            #+#    #+#             */
/*   Updated: 2026/09/21 22:34:13 by mcomin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal.hpp"

volatile sig_atomic_t signalstatus = 0;

void handle_signal(int sig) {
	signalstatus = sig;
}