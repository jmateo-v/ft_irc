/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 11:37:40 by dogs              #+#    #+#             */
/*   Updated: 2026/04/14 16:44:52 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstdlib>
#include <signal.h>
#include "Server.hpp"
#include "SignalHandler.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return(1);
    }
    int port = std::atoi(argv[1]);
    std::string password = argv[2];
    setup_signals();
    signal(SIGPIPE, SIG_IGN);
    try
    {
        Server server(port, password, shutdownFlag);
        server.run();
    }
    catch(const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return (1);
    }
}