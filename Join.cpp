/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:15:37 by agirona           #+#    #+#             */
/*   Updated: 2022/06/06 16:03:37 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Join(std::list<std::string> tab, std::list<Client>::iterator it)
{
	(void)it;
	(void)tab;
	Channel		newone;
	std::list<Client>				tmp;
	std::list<Client>::iterator		update;
	std::list<Client>::iterator		endupdate;
	std::list<Channel>::iterator	itt;

	std::cout << "JOIN" << std::endl;
	if (tab.empty() == true || tab.size() < 1)
		sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("NICK"));
	else if ((itt = findStr<std::list<Channel>, Channel>(_channel, *tab.begin(), &Channel::getName)) != _channel.end())
	{
		if (itt->getName() == *tab.begin())
		{
			if (itt->isInviteOnly() == 1 && itt->isInvited(it->getNick()) == 0)
			{
				sendMessage(it->getFd(), ERR_INVITEONLYCHAN(*tab.begin()));
				return ;
			}
			if (itt->addUser(*it) == 1)
			{
				sendMessage(it->getFd(), ERR_ALREADYJOIN);
				return ;
			}
			sendMessage(it->getFd(), RPL_JOIN(it->getNick(), *tab.begin()));
			sendMessage(it->getFd(), RPL_TOPIC(it->getNick(), *tab.begin(), itt->getTopic()));
			tmp = itt->getAllUser();
			update = tmp.begin();
			endupdate = tmp.end();
			while (update != endupdate)
			{
				sendMessage(update->getFd(), RPL_NAMREPLY(update->getNick(), itt->getName(), itt->getUserList()));
				update++;
			}
			std::cout << "User " << it->getNick() << " joined channel " << *tab.begin() << std::endl;
			return ;
		}
	}
	else
	{
		if (newone.setName(*tab.begin()) == 1)
		{
			sendMessage(it->getFd(), ERR_ERRONEUSCHANNAME(*tab.begin()));
			return ;
		}
		newone.setName(*tab.begin());
		newone.addOperator(*it);
		sendMessage(it->getFd(), RPL_JOIN(it->getNick(), *tab.begin()));
		sendMessage(it->getFd(), RPL_TOPIC(it->getNick(), *tab.begin(), newone.getTopic()));
		sendMessage(it->getFd(), RPL_NAMREPLY(it->getNick(), *tab.begin(), newone.getUserList()));
		_channel.push_back(newone);
	}
}
