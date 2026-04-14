/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:22:32 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/14 17:47:32 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <errno.h>
#include <poll.h>
#include "Client.hpp"
#include "replies.hpp"

Client::Client(int fd, const std::string& hostname, const std::string& password)
: _fd(fd), _hostname(hostname), _password(password),
_passOk(false), _nickSet(false), _userSet(false), _registered(false),
_events(POLLIN)
{}
int Client::getFd() const
{return _fd;}
const std::string& Client::getNick() const
{return _nickname;}
const std::string& Client::getUser() const
{return _username;}
const std::string& Client::getReal() const
{return _realname;}
const std::string& Client::getHostname() const
{return _hostname;}
std::string Client::getPrefix() const
{
	if (!_nickSet || !_userSet)
		return "";
	return _nickname + "!" + _username + "@" + _hostname;
}
short Client::getEvents() const
{ return _events;}
bool Client::isRegistered() const
{
	return _passOk && _nickSet && _userSet;
}
void Client::setNick(const std::string& nick)
{
	if (nick.empty())
		return;
	_nickname = nick;
	_nickSet = true;
}
void Client::setUser(const std::string& user, const std::string& realname)
{
	if (user.empty())
		return;
	_username = user;
	_realname = realname;
	_userSet = true;
}
void Client::setPassword(const std::string& pass)
{ _password = pass;}
void Client::setPassOk()
{
	_passOk = true;
}
void Client::setEvents(short e)
{_events = e;}
void Client::appendRecv(const char* data, size_t len)
{
	size_t newSize = _recvBuffer.size() + len;
	if (newSize > 4096)
	{
		_recvBuffer.clear();
		return;
	}
	_recvBuffer.append(data, len);
}
bool Client::hasLine() const
{
	return _recvBuffer.find("\r\n") != std::string::npos
		|| _recvBuffer.find('\n') != std::string::npos;
}
std::string Client::extractLine()
{
	size_t pos = _recvBuffer.find("\r\n");
	if (pos == std::string::npos)
		pos = _recvBuffer.find('\n');
	//shouldnt reach I think
	if (pos == std::string::npos)
		return "";
	
	std::string line = _recvBuffer.substr(0, pos);
	size_t len = (pos < _recvBuffer.size() && _recvBuffer[pos] == '\r') ? 2 : 1;
	_recvBuffer.erase(0, pos + len);
	return line;
}
void Client::appendSend(const std::string& msg)
{
	_sendBuffer += msg + "\r\n";
	std::cout << "QUEUED: " << msg << " (buffer size: )" << _sendBuffer.size()
	<< ")\n";
}
bool Client::hasPendingSend()
{
	return !_sendBuffer.empty();
}
ssize_t Client::send()
{
	if (_sendBuffer.empty())
		return(0);
	ssize_t bytes = ::send(_fd, _sendBuffer.data(), _sendBuffer.size(),
							MSG_NOSIGNAL | MSG_DONTWAIT);
	if (bytes > 0)
	{
		_sendBuffer.erase(0, bytes);
		std::cout << "Sent " << bytes << " (remain: " << _sendBuffer.size() << ")\n";
	}
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 0;
		std::cout << "Send error: " << strerror(errno) << "\n";
		return -1;
	}
	return bytes;
}
bool Client::tryRegister()
{
	if (_registered)
		return true;
	if (!_passOk || !_nickSet || !_userSet)
		return false;
	_registered = true;
	
	rpl_welcome(_fd, _nickname, _username, _hostname);
	rpl_yourhost(_fd, _nickname);
	rpl_created(_fd, _nickname);
	rpl_myinfo(_fd, _nickname);

	std::cout << "Client " << _fd << " REGISTERED: " << getPrefix() << std::endl;
	return true;
}

