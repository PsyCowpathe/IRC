/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:50:33 by agirona           #+#    #+#             */
/*   Updated: 2022/05/30 20:41:59 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::PartUpdate(std::list<Client>::iterator &sender, const std::list<Channel>::iterator &channel, const std::string &msg)
{
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<Client>					userlist;

	userlist = channel->getAllUser();
	clientIt = userlist.begin();
	clientIte = userlist.end();
	while (clientIt != clientIte)
	{
		if (msg.empty() == false)
			sendMessage(clientIt->getFd(), RPL_PART(sender->getNick(), channel->getName(), msg.c_str()));
		else
			sendMessage(clientIt->getFd(), RPL_EMPTYPART(sender->getNick(), channel->getName()));
		clientIt++;
	}
	if (msg.empty() == false)
		sendMessage(sender->getFd(), RPL_PART(sender->getNick(), channel->getName(), msg.c_str()));
	else
		sendMessage(sender->getFd(), RPL_EMPTYPART(sender->getNick(), channel->getName()));
}

void	Server::Part(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<std::string>::iterator	argsIt;
	std::list<std::string>::iterator	argsIte;
	std::list<Client>					userlist;

	std::cout << "PART" << std::endl;
	chanIt = _channel.begin();
	chanIte = _channel.end();
	argsIt = tab.begin();
	argsIte = tab.end();
	if (tab.empty() == true)
		return (sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("PART")));
	if (chanIt == chanIte)
		return (sendMessage(it->getFd(), ERR_NOSUCHCHANNEL(*tab.begin())));
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == *argsIt)
		{
			if (chanIt->deleteUser(*it) == 0)
			{
				userlist = chanIt->getAllUser();
				if (userlist.size() == 0)
					_channel.erase(chanIt);
				PartUpdate(it, chanIt, *(++argsIt));
				return ;
			}
			else
				return (sendMessage(it->getFd(), ERR_NOTONCHANNEL(chanIt->getName())));
		}
		chanIt++;
	}
	sendMessage(it->getFd(), ERR_NOSUCHCHANNEL(*tab.begin()));
}
