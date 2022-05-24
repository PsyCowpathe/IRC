/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:15:37 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:16:09 by agirona          ###   ########lyon.fr   */
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
		std::cout << "je boucle ici" << std::endl;
		if (itt->getName() == *tab.begin())
		{
			if (itt->addUser(*it) == 1)
			{
				sendMessage(it->getFd(), ERR_ALREADYJOIN);
				return ;
			}
			sendMessage(it->getFd(), RPL_JOIN(it->getNick(), *tab.begin()));
			sendMessage(it->getFd(), RPL_TOPIC(it->getNick(), *tab.begin(), itt->getTopic()));
			//update = itt->getAllUser().begin(); les iterator c fun
			//endupdate = itt->getAllUser().end(); les iterator c fun
			tmp = itt->getAllUser();
			update = tmp.begin();
			endupdate = tmp.end();
			while (update != endupdate)
			{
				std::cout << "je boucle ici 2" << std::endl;
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