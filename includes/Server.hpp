/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 12:28:40 by dogs              #+#    #+#             */
/*   Updated: 2026/04/07 16:56:24 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include "Client.hpp"

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
    std::map <int, Client*> _clients;

    void createSocket();
    void bindSocket();
    void listenSocket();
    void startPollLoop();
    void makeNonBlocking(int fd);
    void disconnectClient(std::vector<pollfd>& pollfds, size_t index);
    Client& getClient(int fd);
};

#endif