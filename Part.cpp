/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:50:33 by agirona           #+#    #+#             */
/*   Updated: 2022/06/08 15:56:15 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::PartUpdate(std::list<Client>::iterator &sender, const std::list<Channel>::iterator &channel, const std::string &msg)
{
	std::list<Client>::iterator            clientIt;
	std::list<Client>::iterator            clientIte;
	std::list<Client>                    userlist;

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

void    Server::Part(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	std::list<Channel>::iterator        chanIt;
	std::list<Channel>::iterator        chanIte;
	std::list<std::string>::iterator    argsIt;
	std::list<std::string>::iterator    argsIte;
	std::list<Client>                   userlist;
	std::list<std::string>				list;
	std::list<std::string>::iterator	listIt;
	std::list<std::string>::iterator	listIte;
	bool								done;

	std::cout << "PART" << std::endl;
	chanIt = _channel.begin();
	chanIte = _channel.end();
	argsIt = (++tab.begin());
	argsIte = tab.end();
	if (tab.empty() == true)
		return (sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("PART")));
	if (chanIt == chanIte)
		return (sendMessage(sender->getFd(), ERR_NOSUCHCHANNEL(*tab.begin())));
	list = cutTab(*tab.begin());
	listIt = list.begin();
	listIte = list.end();
	while (listIt != listIte)
	{
		done = 0;
		chanIt = _channel.begin();
		while (chanIt != chanIte)
		{
			if (chanIt->getName() == *listIt)
			{
				if (chanIt->deleteUser(*sender) == 0)
				{
					userlist = chanIt->getAllUser();
					if (userlist.size() == 0)
						_channel.erase(chanIt);
					if (argsIt == argsIte)
						PartUpdate(sender, chanIt, "");
					else
						PartUpdate(sender, chanIt, *argsIt);
					done = 1;
				}
				else
					sendMessage(sender->getFd(), ERR_NOTONCHANNEL(chanIt->getName()));
			}
			chanIt++;
		}
		if (done == 0)
			sendMessage(sender->getFd(), ERR_NOSUCHCHANNEL(*listIt));
		listIt++;
	}
}
