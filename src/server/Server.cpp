/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 12:40:32 by dogs              #+#    #+#             */
/*   Updated: 2026/03/24 19:12:42 by jmateo-v         ###   ########.fr       */
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
#include <cerrno>
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
                    while (true)
                    {
                        int clientFd = accept(_serverFd, NULL, NULL);
                        if (clientFd < 0)
                        {
                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                                break;
                            if (errno == EINTR)
                                continue;
                            throw std::runtime_error(std::string("Failed to accept connection: ") +
                            std::strerror(errno));
                        }
                    
                        makeNonBlocking(clientFd);

                        pollfd clientPfd;
                        clientPfd.fd = clientFd;
                        clientPfd.events = POLLIN;
                        clientPfd.revents = 0;
                        pollfds.push_back(clientPfd);
                    
                        std::cout << "Client connected on fd " << clientFd << "\n";
                    }
                }
                else
                {
                    int clientFd = pollfds[i].fd;
                    char buffer[512];
                    ssize_t bytes = recv(clientFd, buffer, sizeof(buffer), 0);
                    if (bytes < 0)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            continue;
                        if (errno == EINTR)
                            continue;
                        disconnectClient(pollfds, i);
                        i--;
                        continue;
                    }
                    if (bytes == 0)
                    {
                        disconnectClient(pollfds, i);
                        --i;
                        continue;
                    }
                    if (bytes > 0)
                    {
                        //This will do something ig
                        std::cout << "dogs\n";
                    }
                }
            }
            else if(pollfds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
            {
                disconnectClient(pollfds, i);
                --i;
                continue;
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
void Server::disconnectClient(std::vector<pollfd>& pollfds, size_t index)
{
    int fd = pollfds[index].fd;
    std::cout << "Client " << fd << " disconnected\n";

    close(fd);
    pollfds.erase(pollfds.begin() + index);
}

void Server::run()
{
    createSocket();
    bindSocket();
    listenSocket();
    makeNonBlocking(_serverFd);
    startPollLoop();
}