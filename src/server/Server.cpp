/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 12:40:32 by dogs              #+#    #+#             */
/*   Updated: 2026/04/17 16:06:18 by jmateo-v         ###   ########.fr       */
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
#include <sstream>
#include "Server.hpp"

Server::Server(int port, const std::string& password, volatile sig_atomic_t& flag)
    : _port(port), _password(password), _serverFd(-1),
    _pollFds(), _shutdownFlag(flag)
{
    if (_port < 1 || _port > 65535)
        throw std::runtime_error("Invalid port: must be between 1 and 65535");
    initCommands();
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
        std::strerror(errno));
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
    pollfd serverPfd;
    serverPfd.fd = _serverFd;
    serverPfd.events = POLLIN;
    serverPfd.revents = 0;
    _pollFds.push_back(serverPfd);

    while (_shutdownFlag == 0)
    {
        int ret = poll(_pollFds.data(), _pollFds.size(), -1);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            throw std::runtime_error(std::string("poll failed: ") +
            std::strerror(errno));
        }
        for (size_t i = 0; i < _pollFds.size(); ++i)
        {
            if (_pollFds[i].revents & POLLIN)
            {   
                if (_pollFds[i].fd == _serverFd)
                {
                   handleServerEvent();
                   continue;
                }
                handleClientReadEvent(i);
            }
            else if(_pollFds[i].revents & POLLOUT)
                handleClientWriteEvent(i);
            else if(_pollFds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
            {
                handleClientErrorEvent(i);
                continue;
            }
        }
    }
    exitLoopCleanup();
}
void Server::handleServerEvent()
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
            Client* client = new Client(clientFd, "localhost", _password);
            _clients[clientFd] = client;
             std::cout << "Client connected on fd " << clientFd << "\n";
                        
            pollfd clientPfd;
            clientPfd.fd = clientFd;
            clientPfd.events = POLLIN;
            clientPfd.revents = 0;
            _pollFds.push_back(clientPfd);
    }
}
void Server::handleClientReadEvent(size_t i)
{
    int clientFd = _pollFds[i].fd;
    char buffer[512];
    ssize_t bytes = recv(clientFd, buffer, sizeof(buffer), 0);

    if (bytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        if (errno == EINTR)
            return;
        disconnectClient(i);
        return;
    }
    if (bytes == 0)
    {
        disconnectClient(i);
        return;
    }
    if (bytes > 0)
    {
        Client& client = getClient(clientFd);
        client.appendRecv(buffer, bytes);

        while (client.hasLine())
        {
            std::string line = client.extractLine();
            Message msg = MessageParser::parseMSG(line);
            if (!msg.command.empty())
                dispatchCommand(client, msg);
            short events = POLLIN;
            if (client.hasPendingSend())
                events |= POLLOUT;
            _pollFds[i].events = events;
        }
    }
}
void Server::handleClientWriteEvent(size_t i)
{
    Client& client = getClient(_pollFds[i].fd);
    std::cout << "POLLOUT ready for fd " << _pollFds[i].fd << std::endl;
    client.send();

    short events = POLLIN;
    if (client.hasPendingSend())
        events |= POLLOUT;
    _pollFds[i].events = events;
}
void Server::handleClientErrorEvent(size_t i)
{
    disconnectClient(i);
    --i;
}
void Server::exitLoopCleanup()
{
    std::cout << "Shutdown: Disconnecting " << _pollFds.size() - 1 << " clients...\n";
    for (int i = _pollFds.size() - 1; i > 0; --i)
        disconnectClient(i);
    _pollFds.clear();
    _clients.clear();
    std::cout << "Cleanup complete\n";
}
void Server::makeNonBlocking(int fd)
{
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set socket non-blocking");
}
void Server::disconnectClient(size_t index)
{
    int fd = _pollFds[index].fd;
    std::cout << "Client " << fd << " disconnected\n";

    Client* client = _clients[fd];
    delete client;
    _clients.erase(fd);

    close(fd);
    _pollFds.erase(_pollFds.begin() + index);
}
Client& Server::getClient(int fd)
{
    std::map<int, Client*>::iterator it = _clients.find(fd);
    if (it == _clients.end())
    {
        std::ostringstream oss;
        oss << "Client not found for fd" << fd;
        throw std::runtime_error(oss.str());
    }
    return *it->second;
}
void Server::run()
{
    createSocket();
    bindSocket();
    listenSocket();
    makeNonBlocking(_serverFd);
    startPollLoop();
}