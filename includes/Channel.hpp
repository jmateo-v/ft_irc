/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:13:10 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/24 16:36:14 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <set>
#include "Client.hpp"

class Channel
{
private:
	std::string _name;
	std::string _topic;
	std::string _password;
	std::vector<Client*> _members;
	std::vector<Client*> _operators;
	std::set<Client*> _invited;
	bool _inviteOnly;
	bool _topicLocked;
	bool _passwordSet;
	int _userLimit;
public:
	Channel(std::string name);
	~Channel();
	bool addMember(Client* client, std::string password);
	bool removeMember(Client* client);
	bool hasMember(Client* client) const;
	bool isOperator(Client* client) const;
	void broadcast(const std::string& message, Client* sender);
	const std::string& getChannelname() const;
	const std::string& getChannelTopic() const;
	const std::string& getChannelPasswd() const;
	bool isInviteOnly() const;
	size_t getMemberCount() const;
	void setChannelTopic(const std::string& topic, Client* setter);
	void setPassword(const std::string& password, Client* setter);
	void setInviteOnly(bool inviteOnly, Client* setter);
	bool isTopicLocked() const;
	bool hasPassword() const;
	int getUserLimit() const;
	void setTopicLocked(bool locked, Client* setter);
	void setUserLimit(int limit, Client* setter);
	std::string getModeString() const;
};

#endif