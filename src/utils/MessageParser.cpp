/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 14:26:04 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/24 16:54:16 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageParser.hpp"
#include <sstream>
#include <cctype>

void split_params(const std::string& s, std::vector<std::string>& params)
{
	std::istringstream iss(s);
	std::string param;
	while (iss >> param)
	{
		params.push_back(param);
	}
}

Message MessageParser::parseMSG(const std::string& raw)
{
	Message msg;
	if (raw.empty() || raw.size() > 510)
		return msg;
	
	std::string line = raw;
	bool has_prefix = !line.empty() && line[0] == ':';
	size_t cmd_start = has_prefix ? line.find(' ', 1) + 1 : 0;
	if (cmd_start == std::string::npos)
		cmd_start = 0;

	size_t cmd_end = line.find(' ', cmd_start);
	if (cmd_end == std::string::npos)
		cmd_end = line.size();
	
	std::string cmd_raw = line.substr(cmd_start, cmd_end - cmd_start);
	msg.command.clear();
	std::string::const_iterator it;
	for (it = cmd_raw.begin(); it != cmd_raw.end(); ++it)
    	msg.command += std::toupper(static_cast<unsigned char>(*it));
	std::string param_part = (cmd_end < line.size()) ? line.substr(cmd_end + 1) : "";
	size_t trail_pos = param_part.rfind(" :");
	if (trail_pos != std::string::npos)
	{
		std::string pre = param_part.substr(0, trail_pos);
		split_params(pre, msg.params);
		msg.params.push_back(param_part.substr(trail_pos + 2));
	}
	else
		split_params(param_part, msg.params);
	if (has_prefix)
	{
		size_t prefix_end = line.find(' ', 0);
		if (prefix_end != std::string::npos)
			msg.prefix = line.substr(1, prefix_end - 1);
	}
	return msg;
}
