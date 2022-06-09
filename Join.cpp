/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:15:37 by agirona           #+#    #+#             */
/*   Updated: 2022/06/09 18:32:59 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Join(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	Channel								newone;
	std::list<Client *>					tmp;
	std::list<Client *>::iterator		update;
	std::list<Client *>::iterator		endupdate;
	std::list<Channel>::iterator		itt;
	std::list<std::string>				list;
	std::list<std::string>::iterator	listIt;
	std::list<std::string>::iterator	listIte;

	std::cout << "JOIN" << std::endl;
	if (tab.empty() == true || tab.size() < 1)
	{
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("NICK"));
		return ;
	}
	list = cutTab(*tab.begin());
	listIt = list.begin();
	listIte = list.end();
	while (listIt != listIte)
	{
		*listIt = downgrade(listIt->c_str());
		if ((itt = findStr<std::list<Channel>, Channel>(_channel, *listIt, &Channel::getName)) != _channel.end())
		{
			if (itt->getName() == *listIt)
			{
				if (itt->isInviteOnly() == 1 && itt->isInvited(sender->getNick()) == 0)
					sendMessage(sender->getFd(), ERR_INVITEONLYCHAN(*listIt));
				else if (itt->addUser(&(*sender)) == 1)
					sendMessage(sender->getFd(), ERR_ALREADYJOIN);
				else
				{
					sendMessage(sender->getFd(), RPL_JOIN(sender->getNick(), *listIt));
					sendMessage(sender->getFd(), RPL_TOPIC(sender->getNick(), *listIt, itt->getTopic()));
					tmp = itt->getAllUser();
					update = tmp.begin();
					endupdate = tmp.end();
					while (update != endupdate)
					{
						sendMessage((*update)->getFd(), RPL_NAMREPLY((*update)->getNick(), itt->getName(), itt->getUserList()));
						update++;
					}
					std::cout << "User " << sender->getNick() << " joined channel " << *listIt << std::endl;
				}
			}
		}
		else
		{
			if (newone.setName(*listIt) == 1)
				sendMessage(sender->getFd(), ERR_ERRONEUSCHANNAME(*listIt));
			else
			{
				newone.setName(*listIt);
				newone.addOperator(&(*sender));
				sendMessage(sender->getFd(), RPL_JOIN(sender->getNick(), *listIt));
				sendMessage(sender->getFd(), RPL_TOPIC(sender->getNick(), *listIt, newone.getTopic()));
				sendMessage(sender->getFd(), RPL_NAMREPLY(sender->getNick(), *listIt, newone.getUserList()));
				_channel.push_back(newone);
				std::cout << "User " << sender->getNick() << " created channel " << *listIt << std::endl;
			}
		}
		listIt++;
	}
}
