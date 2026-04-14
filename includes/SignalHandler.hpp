/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:26:14 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/14 16:37:24 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP

#include <csignal>

extern volatile sig_atomic_t shutdownFlag;
void setup_signals();

#endif