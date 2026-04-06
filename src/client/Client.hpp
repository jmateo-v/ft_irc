/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogs <dogs@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 11:39:31 by dogs              #+#    #+#             */
/*   Updated: 2026/03/27 11:52:44 by dogs             ###   ########.fr       */
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

    bool _passOk;
    bool _nickSet;
    bool _userSet;
    bool _registered;

public:
    Client(int fd, const std::string& hostname);
    int getFd();
    std::string getNick();
    std::string getUser();
    std::string getReal();
    std::string getHostname();
    std::string getPrefix();
    bool isRegistered();

    void setNick(const std::string& nick);
    void setUser(const std::string& user, const std::string _realname);
    void setPassOk();

};

#endif