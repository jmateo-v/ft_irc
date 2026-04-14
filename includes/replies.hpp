/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:17:27 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/14 18:01:59 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_NAME "OurIRC.com"

inline void sendNumeric(int fd, int numeric, const std::string& msg)
{
    char numbuf[4];
    
    numbuf[0] = '0' + (numeric / 100);
    numbuf[1] = '0' + ((numeric / 10) % 10);
    numbuf[2] = '0' + (numeric % 10);
    numbuf[3] = '\0';
    
    std::string full = ":" + std::string(SERVER_NAME) + " " 
                       + numbuf + " " + msg + "\r\n";
    send(fd, full.c_str(), full.length(), 0);
}
//RPL_001 WELCOME
inline void rpl_welcome(int fd, const std::string& nick, const std::string& user, const std::string& host)
{
	std::string prefix = nick + "!" + user + "@" + host;
	sendNumeric(fd, 1, prefix + " :Welcome to the IRC network " + nick);
}
//RPL_002 YOURHOST
inline void rpl_yourhost(int fd, const std::string&nick)
{
	std::string msg = nick + " :Your host is " + SERVER_NAME + " running version 1.0";
	sendNumeric(fd, 2, msg);
}
//RPL_003 CREATED
inline void rpl_created(int fd, const std::string& nick)
{
	std::string msg = nick + " :This server was created today"; //maybe put an actual date tracker idk?
	sendNumeric(fd, 3, msg);
}
//RPL_004 MYINFO
inline void rpl_myinfo(int fd, const std::string& nick)
{
	std::string msg = nick + " " + SERVER_NAME + " 1.0 (no modes yet)";
	sendNumeric(fd, 4, msg);
}

//ERRORS 
inline void err_nonicknamegiven(int fd, const std::string& nick)
{
    sendNumeric(fd, 431, nick + " :No nickname given");
}

inline void err_erroneusnickname(int fd, const std::string& nick, const std::string& badnick)
{
    sendNumeric(fd, 432, nick + " " + badnick + " :Erroneus nickname");
}

inline void err_nicknameinuse(int fd, const std::string& nick)
{
    sendNumeric(fd, 433, nick + " :Nickname is already in use");
}

inline void err_needmoreparams(int fd, const std::string& nick, const std::string& cmd)
{
    sendNumeric(fd, 461, nick + " " + cmd + " :Not enough parameters");
}

inline void err_alreadyregistered(int fd, const std::string& nick)
{
    sendNumeric(fd, 462, nick + " :You may not reregister");
}

inline void err_passwdmismatch(int fd, const std::string& nick)
{
    sendNumeric(fd, 464, nick + " :Password incorrect");
}
#endif