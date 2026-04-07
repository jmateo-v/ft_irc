/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 11:39:31 by dogs              #+#    #+#             */
/*   Updated: 2026/04/07 17:24:55 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    std::string _password;
    std::string _recvBuffer;
    std::string _sendBuffer;
    bool _passOk;
    bool _nickSet;
    bool _userSet;
    bool _registered;
    short _events;

public:
    Client(int fd, const std::string& hostname, const std::string& password);
    
    int getFd() const;
    const std::string& getNick() const;
    const std::string& getUser() const;
    const std::string& getReal() const;
    const std::string& getHostname() const;
    std::string getPrefix() const;
    short getEvents() const;
    bool isRegistered() const;

    void setNick(const std::string& nick);
    void setUser(const std::string& user, const std::string& realname);
    void setPassword(const std::string& pass);
    void setPassOk();
    void setEvents(short e);

    void appendRecv(const char *data, size_t len);
    bool hasLine() const;
    std::string extractLine();

    void appendSend(const std::string& msg);
    bool hasPendingSend();
    ssize_t send();

    bool tryRegister();
};

#endif