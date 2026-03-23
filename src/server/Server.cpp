/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogs <dogs@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 12:40:32 by dogs              #+#    #+#             */
/*   Updated: 2026/03/23 13:13:45 by dogs             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include "Server.hpp"

Server::Server(int port, const std::string& password)
    : _port(port), _password(password), _serverFd(-1)
{
    if (_port < 1 || _port > 65535)
        throw std::runtime_error("Invalid port: must be between 1 and 65535");
}
Server::~Server()
{
    if (_serverFd != -1)
    {
        std::cout << "Debug: Closing socket with fd " << _serverFd << "\n";
        close(_serverFd);
    }
}

void Server::createSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw std::runtime_error(std::string("Failed to create the socket: ") +
        std::strerror(errno));
    
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error(std::string("Failed to set SO_REUSEADDR: ") +
        std::strerror(errno));;
}
void Server::bindSocket()
{
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_port);

    if (bind(_serverFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error(std::string("Failed to bind to the socket: ") +
        std::strerror(errno));
}
void Server::listenSocket()
{
    if (listen(_serverFd, 10) < 0)
        throw std::runtime_error(std::string("Failed to listen on the socket: ") +
        std::strerror(errno));
}

void Server::startPollLoop()
{
    std::vector<pollfd> pollfds;
    pollfd serverPfd;
    serverPfd.fd = _serverFd;
    serverPfd.events = POLLIN;
    serverPfd.revents = 0;
    pollfds.push_back(serverPfd);

    while (true)
    {
        int ret = poll(pollfds.data(), pollfds.size(), -1);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            throw std::runtime_error(std::string("poll failed: ") +
            std::strerror(errno));
        }
        for (size_t i = 0; i < pollfds.size(); ++i)
        {
            if (pollfds[i].revents & POLLIN)
            {
                if (pollfds[i].fd == _serverFd)
                {
                    std::cout << "Something attempted connection o algo\n";
                    //accept logic goes here
                }
            }
        }
    }
}

void Server::makeNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("Failed to retrieve socket flags");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set socket flags");
}

void Server::run()
{
    createSocket();
    bindSocket();
    listenSocket();
    makeNonBlocking(_serverFd);
    startPollLoop();
}