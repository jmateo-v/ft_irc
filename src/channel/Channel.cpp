/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:34:05 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/24 18:23:08 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <map>
#include "Channel.hpp"

Channel::Channel(std::string name)
: _name(name), _topic(""), _password(""),
  _members(), _operators(),
  _invited(), _inviteOnly(false),
  _topicLocked(true), _passwordSet(false),
  _userLimit(0)
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

std::string Channel::getPrefixedNicks() const
{
    std::string nicks;
    std::vector<Client*>::const_iterator it;

    for (it = _operators.begin(); it != _operators.end(); ++it)
	{
        nicks += "@";
        nicks += (*it)->getNick();
        nicks += " ";
    }

    for (it = _members.begin(); it != _members.end(); ++it)
	{
        if (!isOperator(*it))
		{
            nicks += (*it)->getNick();
            nicks += " ";
        }
    }
    
    if (!nicks.empty())
        nicks = nicks.substr(0, nicks.length() - 1);
    return nicks;
}
bool Channel::isOperator(Client* client) const
{
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}
bool Channel::hasMember(Client* client) const
{
    return std::find(_members.begin(), _members.end(), client) != _members.end();
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
	if (_userLimit > 0 && static_cast<int>(_members.size()) >= _userLimit)
		return false;
	_members.push_back(client);
	if (_operators.empty())
		_operators.push_back(client);
	return true;
}
bool Channel::removeMember(Client* client)
{
    std::vector<Client*>::iterator memberIt = std::find(_members.begin(), _members.end(), client);
    if (memberIt != _members.end())
        _members.erase(memberIt);
    else
		return false;
    std::vector<Client*>::iterator opIt = std::find(_operators.begin(), _operators.end(), client);
    if (opIt != _operators.end())
        _operators.erase(opIt);
	else
		return false;
    
    _invited.erase(client);
    return true;
}
bool Channel::isTopicLocked() const
{return _topicLocked;}

bool Channel::hasPassword() const
{return _passwordSet;}

int Channel::getUserLimit() const
{return _userLimit;}

void Channel::setTopicLocked(bool locked, Client* setter)
{
	if (!isOperator(setter))
		return;
	_topicLocked = locked;
}
void Channel::setUserLimit(int limit, Client* setter)
{
    if (!isOperator(setter))
		return;
    _userLimit = (limit > 0) ? limit : 0;
}
std::string Channel::getModeString() const
{
    std::string modes = "t";
    if (_inviteOnly) modes += "i";
    if (_passwordSet) modes += "k";
    if (_userLimit > 0) modes += "l";
    return modes;
}