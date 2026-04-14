/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:20:14 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/14 18:24:25 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "replies.hpp"
#include <iostream>

void Server::initCommands()
{
    //INITIALIZE FUTURE COMMANDS
    _cmdMap["PASS"] = &Server::handlePass;
    _cmdMap["NICK"] = &Server::handleNick;
    _cmdMap["USER"] = &Server::handleUser;
}

void Server::dispatchCommand(Client& client, const Message& msg)
{
    std::map<std::string, CmdFunc>::iterator it = _cmdMap.find(msg.command);
    if (it != _cmdMap.end())
    {
        std::cout << "Dispatching: " << msg.command << std::endl;
        (this->*(it->second))(client, msg.params);
    }
    else
        std::cout << "Unknown: " << msg.command << std::endl;
}
void Server::handlePass(Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		err_needmoreparams(client.getFd(), client.getNick().empty() ? "*" : client.getNick(), "PASS");
		return;
	}
	client.setPassword(params[0]);
	if (params[0] == _password)
	{
		client.setPassOk();
		client.tryRegister();
		std::cout << "Pass OK for " << client.getFd() << std::endl;
	}
	else
	{
		err_passwdmismatch(client.getFd(), client.getNick().empty() ? "*" : client.getNick());
		std::cout << "Pass wrong " << client.getFd() << std::endl;
		//maybe disconnect idk
	}
}
void Server::handleNick(Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		err_nonicknamegiven(client.getFd(), client.getNick().empty() ? "*" : client.getNick());
		return;
	}
	std::string newnick = params[0];
	if (newnick.empty() || newnick.size() > 9) //should implement an isValid()
	{
		err_erroneusnickname(client.getFd(), client.getNick().empty() ? "*" : client.getNick(), newnick);
		return;
	}
	//CHECK FOR DUPE NICK LATER
	//ERROR 433 goes here
	client.setNick(newnick);
	client.tryRegister();
	//ERROR 462 logic for already registering
}
void Server::handleUser(Client& client, const std::vector<std::string>& params)
{
	if (params.size() < 4)
	{
		err_needmoreparams(client.getFd(), client.getNick().empty() ? "*" : client.getNick(), "USER");
		return;
	}
	std::string username = params[0];
	std::string realname = params[3];

	if(username.empty() || username.size() > 10)
	{
		err_needmoreparams(client.getFd(), client.getNick().empty() ? "*" : client.getNick(), "USER");
		return;
	}
	client.setUser(username, realname);
	client.tryRegister();
}