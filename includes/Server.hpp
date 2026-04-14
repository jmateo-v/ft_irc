/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 12:28:40 by dogs              #+#    #+#             */
/*   Updated: 2026/04/14 16:44:39 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include <csignal> 
#include "Client.hpp"
#include "MessageParser.hpp"

class Server
{
public:
    Server(int port, const std::string &password, volatile sig_atomic_t& flag);
    ~Server();

    void run();
private:

    int _port;
    std::string _password;
    int _serverFd;
    std::map <int, Client*> _clients;
    typedef void (Server::*CmdFunc)(Client&, const std::vector<std::string>&);
    std::map<std::string, CmdFunc>   _cmdMap;
    volatile sig_atomic_t& _shutdownFlag;
    
    void createSocket();
    void bindSocket();
    void listenSocket();
    void startPollLoop();
    void makeNonBlocking(int fd);
    void disconnectClient(std::vector<pollfd>& pollfds, size_t index);
    Client& getClient(int fd);
    void initCommands();
    void dispatchCommand(Client& client, const Message& msg);

    void handlePass(Client& client, const std::vector<std::string>& params);
    void handleNick(Client& client, const std::vector<std::string>& params);
    void handleUser(Client& client, const std::vector<std::string>& params);

};

#endif