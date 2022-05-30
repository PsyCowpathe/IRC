/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandList.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:14:31 by agirona           #+#    #+#             */
/*   Updated: 2022/05/30 14:33:58 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string     Server::commandList[] =
{
	"JOIN",
	"PRIVMSG",
	"PING",
	"NICK",
	"USER",
	"PASS",
	"PART",
};

void    (Server::*(Server::commandFct)[])(std::list<std::string> tab, std::list<Client>::iterator it) =
{
	&Server::Join,
	&Server::privMsg,
	&Server::Ping,
	&Server::Nick,
	&Server::User,
	&Server::User,
	&Server::Part,
};

void	Server::detectCommand(std::list<Client>::iterator it, const std::string &buff)
{
	int		i;
	int		valid;
	std::list<std::string>	tab;

	i = 0;
	valid = 0;
	while (i < _nbcommand)
	{
		if (cutdeBuff(&tab, buff, commandList[i]) == 1)
		{
			valid = 1;
			(this->*commandFct[i])(tab, it);
		}
		i++;
	}
	if (valid == 0)
		sendMessage(it->getFd(), ERR_INVALIDCOMMAND);
}
