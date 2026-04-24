/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:24:03 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/24 18:44:52 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "replies.hpp"
#include <sstream>

void Server::handleJoin(Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		err_notregistered(client.getFd(), client.getNick().empty() ? "*" : client.getNick());
		return;
	}
	if (params.empty())
	{
		err_needmoreparams(client.getFd(),client.getNick(), "JOIN");
		return;
	}

	std::string channelName = params[0];
	std::string key = (params.size() > 1 ? params[1] : "");

	if (channelName[0] != '#' && channelName[0] != '&')
	{
		err_nosuchchannel(client.getFd(), client.getNick(), channelName);
		return;
	}

	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		channel = new Channel(channelName);
		_channels[channelName] = channel;
	}
	if (!channel->addMember(&client, key))
	{
		if (channel->isInviteOnly())
			err_inviteonlychan(client.getFd(), client.getNick(), channelName);
		else if (!channel->getChannelPasswd().empty())
			err_badchannelkey(client.getFd(), client.getNick(), channelName);
		else if (static_cast<int>(channel->getMemberCount()) >= channel->getUserLimit())
			err_channelisfull(client.getFd(), client.getNick(), channelName);
		else
			err_useronchannel(client.getFd(), client.getNick(), channelName);
		return;
	}
	std::string symbol = (channelName[0] == '#') ? "=" : "*";
	//Broadcast join
	if (!channel->getChannelTopic().empty())
		rpl_topic(client.getFd(), client.getNick(), channelName, channel->getChannelTopic());
	rpl_namreply(client.getFd(), symbol, channelName, channel->getPrefixedNicks());
	rpl_endofnames(client.getFd(), client.getNick(), channelName);
}
void Server::handlePart(Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		err_notregistered(client.getFd(), client.getNick().empty() ? "*" : client.getNick());
		return;
	}
	if (params.empty())
	{
		err_needmoreparams(client.getFd(),client.getNick(), "PART");
		return;
	}
	
	std::string channels = params[0];
	std::vector<std::string> channelList;

	std::stringstream ss(channels);
	std::string token;
	
	while (std::getline(ss, token, ','))
		channelList.push_back(token);
	
	for (std::vector<std::string>::iterator it = channelList.begin();
		it != channelList.end(); it++)
	{
		std::string channelName = *it;

		Channel* channel = findChannel(channelName);
		if (!channel)
		{
			err_nosuchchannel(client.getFd(), client.getNick(), channelName);
            continue;
		}
		if (!channel->hasMember(&client))
		{
			err_notonchannel(client.getFd(), client.getNick(), channelName);
			continue;
		}
		channel->removeMember(&client);
		//broadcast goes here
	}
}
