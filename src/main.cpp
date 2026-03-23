/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogs <dogs@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 11:37:40 by dogs              #+#    #+#             */
/*   Updated: 2026/03/19 13:49:43 by dogs             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstdlib>
#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return(1);
    }
    int port = std::atoi(argv[1]);
    std::string password = argv[2];
    try
    {
        Server server(port, password);
        server.run();
    }
    catch(const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return (1);
    }
}