/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:13:10 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/17 18:12:39 by jmateo-v         ###   ########.fr       */
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
public:
	Channel(std::string name);
	~Channel();
	bool addMember(Client* client, std::string password);
	bool removeUser(Client* client);
	bool isOperator(Client* client) const;
	void broadcast(const std::string& message, Client* sender);
	const std::string& getChannelname() const;
	const std::string& getChannelTopic() const;
	const std::string& getChannelPasswd() const;
	bool isInviteOnly() const;
	bool isOperator(Client* member) const;
	size_t getMemberCount() const;
	void setChannelTopic(const std::string& topic, Client* setter);
	void setPassword(const std::string& password, Client* setter);
	void setInviteOnly(bool inviteOnly, Client* setter);
};

#endif