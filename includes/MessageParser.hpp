/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmateo-v <jmateo-v@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 14:13:14 by jmateo-v          #+#    #+#             */
/*   Updated: 2026/04/14 14:19:54 by jmateo-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEPARSER_HPP
#define MESSAGEPARSER_HPP

#include <string>
#include <vector>

struct Message
{
	std::string prefix;
	std::string command;
	std::vector<std::string> params;
};

class MessageParser
{
public:
	static Message parseMSG(const std::string& raw);
};

#endif