/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 16:03:50 by agirona           #+#    #+#             */
/*   Updated: 2022/06/13 15:49:40 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::KickUpdate(const std::list<Client>::iterator &sender, const std::string &target, const std::list<Channel>::iterator &channel, const std::string &msg)
{
	std::list<Client *>					list;
	std::list<Client *>::iterator			clientIt;
	std::list<Client *>::iterator			clientIte;

	list = channel->getAllUser();
	clientIt = list.begin();
	clientIte = list.end();
	while (clientIt != clientIte)
	{
		sendMessage((*clientIt)->getFd(), RPL_KICK(sender->getNick(), channel->getName(), target, msg));
		clientIt++;
	}
}

void	Server::Kick(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	std::list<std::string>::iterator	argsIt;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::string							channel;
	std::string							target;
	std::string							msg;
	std::list<std::string>				list;
	std::list<std::string>::iterator	listIt;
	std::list<std::string>::iterator	listIte;
	bool								done;

	std::cout << "KICK" << std::endl;
	if (tab.size() < 2)
	{
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("MODE"));
		return ;
	}
	chanIte = _channel.end();
	argsIt = tab.begin();
	channel = downgrade(argsIt->c_str());
	target = downgrade((++argsIt)->c_str());
	msg = *(++argsIt);
	list = cutTab(target);
	listIt = list.begin();
	listIte = list.end();
	while (listIt != listIte)
	{
		chanIt = _channel.begin();
		done = 0;
		while (chanIt != chanIte)
		{
			if (chanIt->getName() == channel)
			{
				done = 1;
				if (chanIt->isOp(sender->getNick()) == 1)
				{
					if (chanIt->isJoin(*listIt) == 1)
					{
						if (tab.size() == 2)
							KickUpdate(sender, *listIt, chanIt, "");
						else
							KickUpdate(sender, *listIt, chanIt, msg.c_str());
						chanIt->deleteOperator(chanIt->findUser(*listIt));
						chanIt->deleteUser(chanIt->findUser(*listIt));
					}
					else
						sendMessage(sender->getFd(), ERR_NOTINCHANNEL(*tab.begin(), channel));
				}
				else
				{
					sendMessage(sender->getFd(), ERR_CHANOPPRIVSNEEDED(channel));
					return ;
				}
			}
			chanIt++;
		}
		if (done == 0)
			sendMessage(sender->getFd(), ERR_NOSUCHCHANNEL(*tab.begin()));
		listIt++;
	}
}
