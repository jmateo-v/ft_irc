/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:31:00 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/14 16:41:36 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "SignalHandler.hpp"

volatile sig_atomic_t shutdownFlag = 0;

extern "C" void signal_handler(int, siginfo_t*, void*)
{
	shutdownFlag = 1;
	const char msg[] = "Shutting down...\n";
	write(STDERR_FILENO, msg, sizeof(msg)-1);
}
void setup_signals()
{
    struct sigaction sa = {};
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}