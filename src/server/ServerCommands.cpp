/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:20:14 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/14 15:54:14 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

void Server::handlePass(Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		client.appendSend("461 * PASS :Not enough parameters");
		return;
	}
	client.setPassword(params[0]);
	if (params[0] == _password)
	{
		client.setPassOk();
		client.tryRegister();
		std::cout << "Pass OK for" << client.getFd() << std::endl;
	}
	else
		std::cout << "Pass wrong" << client.getFd() << std::endl;
}
void Server::handleNick(Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		client.appendSend("431 * :No nickname given");
		return;
	}
	std::string newnick = params[0];
	if (newnick.empty() || newnick.size() > 9) //should implement an isValid()
	{
		client.appendSend("432 * " + newnick + " :Erroneus nickname");
		return;
	}
	//CHECK FOR DUPE NICK LATER
	client.setNick(newnick);
	client.tryRegister();
}
void Server::handleUser(Client& client, const std::vector<std::string>& params)
{
	if (params.size() < 4)
	{
		client.appendSend("461 * USER :Not enough parameters");
		return;
	}
	std::string username = params[0];
	std::string realname = params[3];

	if(username.empty() || username.size() > 10)
	{
		client.appendSend("461 * USER :Invalid username");
		return;
	}
	client.setUser(username, realname);
	client.tryRegister();
}