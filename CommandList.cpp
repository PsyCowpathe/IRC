/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandList.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:14:31 by agirona           #+#    #+#             */
/*   Updated: 2022/06/07 19:01:56 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string     Server::commandList[] =
{
	"JOIN", //decoupe les chan
	"PRIVMSG", //decoupe les targets
	"PING",
	"NICK", 
	"USER",
	"PASS",
	"PART", //decoupe les chan
	"MODE",
	"INVITE",
	"NOTICE", //decoupel les target
	"TOPIC",
	"KICK", //decoupe les target
};

void    (Server::*(Server::commandFct)[])(std::list<std::string> tab, std::list<Client>::iterator sender) =
{
	&Server::Join,
	&Server::privMsg,
	&Server::Ping,
	&Server::Nick,
	&Server::User,
	&Server::User,
	&Server::Part,
	&Server::Mode,
	&Server::Invite,
	&Server::Notice,
	&Server::Topic,
	&Server::Kick,
};

void	Server::detectCommand(std::list<Client>::iterator sender, const std::string &buff)
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
			std::cout << "Command detecter" << std::endl;
			valid = 1;
			(this->*commandFct[i])(tab, sender);
		}
		i++;
	}
	if (valid == 0)
		sendMessage(sender->getFd(), ERR_INVALIDCOMMAND);
}
