/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogs <dogs@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 12:28:40 by dogs              #+#    #+#             */
/*   Updated: 2026/03/23 12:20:33 by dogs             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

class Server
{
public:
    Server(int port, const std::string &password);
    ~Server();

    void run();
private:

    int _port;
    std::string _password;
    int _serverFd;

    void createSocket();
    void bindSocket();
    void listenSocket();
    void startPollLoop();
    void makeNonBlocking(int fd);

    Server(); //3 useless functions (DO NOT DEFINE)
    Server(const Server&); //for subject compliance
    Server& operator=(const Server&); //not sure, leaving them here for now ig
};

#endif