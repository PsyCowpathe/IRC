/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:15:37 by agirona           #+#    #+#             */
/*   Updated: 2022/06/07 18:56:08 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Join(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	Channel		newone;
	std::list<Client>				tmp;
	std::list<Client>::iterator		update;
	std::list<Client>::iterator		endupdate;
	std::list<Channel>::iterator	itt;

	std::cout << "JOIN" << std::endl;
	if (tab.empty() == true || tab.size() < 1)
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("NICK"));
	else if ((itt = findStr<std::list<Channel>, Channel>(_channel, *tab.begin(), &Channel::getName)) != _channel.end())
	{
		if (itt->getName() == *tab.begin())
		{
			if (itt->isInviteOnly() == 1 && itt->isInvited(sender->getNick()) == 0)
			{
				sendMessage(sender->getFd(), ERR_INVITEONLYCHAN(*tab.begin()));
				return ;
			}
			if (itt->addUser(*sender) == 1)
			{
				sendMessage(sender->getFd(), ERR_ALREADYJOIN);
				return ;
			}
			sendMessage(sender->getFd(), RPL_JOIN(sender->getNick(), *tab.begin()));
			sendMessage(sender->getFd(), RPL_TOPIC(sender->getNick(), *tab.begin(), itt->getTopic()));
			tmp = itt->getAllUser();
			update = tmp.begin();
			endupdate = tmp.end();
			while (update != endupdate)
			{
				sendMessage(update->getFd(), RPL_NAMREPLY(update->getNick(), itt->getName(), itt->getUserList()));
				update++;
			}
			std::cout << "User " << sender->getNick() << " joined channel " << *tab.begin() << std::endl;
			return ;
		}
	}
	else
	{
		if (newone.setName(*tab.begin()) == 1)
		{
			sendMessage(sender->getFd(), ERR_ERRONEUSCHANNAME(*tab.begin()));
			return ;
		}
		newone.setName(*tab.begin());
		newone.addOperator(*sender);
		sendMessage(sender->getFd(), RPL_JOIN(sender->getNick(), *tab.begin()));
		sendMessage(sender->getFd(), RPL_TOPIC(sender->getNick(), *tab.begin(), newone.getTopic()));
		sendMessage(sender->getFd(), RPL_NAMREPLY(sender->getNick(), *tab.begin(), newone.getUserList()));
		_channel.push_back(newone);
	}
}
