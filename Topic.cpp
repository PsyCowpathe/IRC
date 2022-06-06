/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:36:31 by agirona           #+#    #+#             */
/*   Updated: 2022/06/06 17:59:38 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//<chan> <topic>

void	Server::TopicUpdate(std::list<Client>::iterator &sender, const std::list<Channel>::iterator &channel)
{
	std::list<Client>					list;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;


	list = channel->getAllUser();
	clientIt = list.begin();
	clientIte = list.end();
	while (clientIt != clientIte)
	{
		sendMessage(clientIt->getFd(), RPL_TOPIC(sender->getNick(), channel->getName(), channel->getTopic()));
		clientIt++;
	}
}

void	Server::Topic(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::list<std::string>::iterator	argsIt;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<Client>					list;

	std::cout << "TOPIC" << std::endl;
	if (tab.size() < 1)
	{
		sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("MODE"));
		return ;
	}
	chanIt = _channel.begin();
	chanIte = _channel.end();
	argsIt = tab.begin();
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == *argsIt)
		{
			if (tab.size() == 1)
				sendMessage(it->getFd(), RPL_TOPIC(it->getNick(), *argsIt, chanIt->getTopic()));
			else if (chanIt->isOp(it->getNick()) == 1)
			{
				chanIt->setTopic(*(++argsIt));
				TopicUpdate(it, chanIt);
			}
			else
				sendMessage(it->getFd(), ERR_CHANOPPRIVSNEEDED(*(argsIt)));
			return ;
		}
		chanIt++;
	}
	sendMessage(it->getFd(), ERR_NOSUCHCHANNEL(*tab.begin()));
}
