/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:34:05 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/17 18:12:20 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "Channel.hpp"

Channel::Channel(std::string name)
: _name(name), _topic(""), _password(""),
  _members(), _operators(),
  _invited(), _inviteOnly(false)
{}
Channel::~Channel()
{}
const std::string& Channel::getChannelname() const
{return _name;}
const std::string& Channel::getChannelTopic() const
{return _topic;}
const std::string& Channel::getChannelPasswd() const
{return _password;}
bool Channel::isInviteOnly() const
{return _inviteOnly;}
size_t Channel::getMemberCount() const
{return _members.size();}
bool Channel::isOperator(Client* client) const
{
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}
void Channel::setChannelTopic(const std::string& topic, Client* setter)
{
	if (!isOperator(setter))
		return;
	_topic = topic;
	//broadcast later same as above
}
void Channel::setPassword(const std::string& password, Client* setter)
{
	if (!isOperator(setter))
		return;
	_password = password;
	//broadcast +k
}
void Channel::setInviteOnly(bool inviteOnly, Client* setter)
{
	if (!isOperator(setter))
		return;
	_inviteOnly = inviteOnly;
	//broadcast +i/-i
}
bool Channel::addMember(Client* client, std::string password)
{
	if (std::find(_members.begin(), _members.end(), client) != _members.end())
		return false;
	if (!_password.empty() && password != _password)
		return false;
	if (_inviteOnly && std::find(_invited.begin(), _invited.end(), client) == _invited.end())
		return false;
	_members.push_back(client);
	if (_operators.empty())
		_operators.push_back(client);
	return true;
}