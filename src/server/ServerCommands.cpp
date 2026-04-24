/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:20:14 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/24 18:58:12 by jmateo-v         ###   ########.fr       */
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
	_cmdMap["JOIN"] = &Server::handleJoin;
	_cmdMap["PART"] = &Server::handlePart;
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
	{
        std::cout << "Unknown: " << msg.command << std::endl;
		err_unknowncommand(client.getFd(), client.getNick().empty() ? "*" : client.getNick(), msg.command);
	}
}
void Server::handlePass(Client& client, const std::vector<std::string>& params)
{
	if (client.isRegistered())
	{
		err_alreadyregistered(client.getFd(), client.getNick());
		return;
	}
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
	if (!isValidNick(newnick))
	{
		err_erroneusnickname(client.getFd(), client.getNick().empty() ? "*" : client.getNick(), newnick);
		return;
	}
	if (findClientByNick(newnick))
	{
		err_nicknameinuse(client.getFd(), newnick);
		return;
	}
	client.setNick(newnick);
	if (client.isRegistered())
		; //broadcast here nick change
	else
		client.tryRegister();
}
void Server::handleUser(Client& client, const std::vector<std::string>& params)
{
	if (client.isRegistered())
	{
		err_alreadyregistered(client.getFd(), client.getNick());
		return;
	}
	if (params.size() < 4)
	{
		err_needmoreparams(client.getFd(), client.getNick().empty() ? "*" : client.getNick(), "USER");
		return;
	}
	std::string username = params[0];
	std::string realname = params[3];

	if(!isValidUser(username))
	{
		err_erroneususername(client.getFd(),"*", username);
		return;
	}
	client.setUser(username, realname);
	client.tryRegister();
}