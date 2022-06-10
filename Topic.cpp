/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:36:31 by agirona           #+#    #+#             */
/*   Updated: 2022/06/10 19:25:00 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//<chan> <topic>

void	Server::TopicUpdate(std::list<Client>::iterator &sender, const std::list<Channel>::iterator &channel)
{
	std::list<Client *>					list;
	std::list<Client *>::iterator			clientIt;
	std::list<Client *>::iterator			clientIte;
	bool								empty;


	list = channel->getAllUser();
	clientIt = list.begin();
	clientIte = list.end();
	empty = 0;
	if (channel->getTopic().size() == 0)
		empty = 1;
	while (clientIt != clientIte)
	{
		if (empty == true)
			sendMessage((*clientIt)->getFd(), RPL_EMPTYTOPIC(sender->getNick(), channel->getName()));
		else
			sendMessage((*clientIt)->getFd(), RPL_TOPIC(sender->getNick(), channel->getName(), channel->getTopic()));
		clientIt++;
	}
}

void	Server::Topic(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	std::list<std::string>::iterator	argsIt;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<Client>					list;
	std::string							channame;
	std::string							topic;

	std::cout << "TOPIC" << std::endl;
	if (tab.size() < 1)
	{
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("MODE"));
		return ;
	}
	chanIt = _channel.begin();
	chanIte = _channel.end();
	argsIt = tab.begin();
	channame = downgrade((argsIt++)->c_str());
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == channame)
		{
			if (tab.size() == 1)
			{
				if (chanIt->getTopic().size() == 0)
					sendMessage(sender->getFd(), RPL_EMPTYTOPIC(sender->getNick(), channame));
				else
					sendMessage(sender->getFd(), RPL_TOPIC(sender->getNick(), channame, chanIt->getTopic()));
			}
			else if (chanIt->isOp(sender->getNick()) == 1)
			{
				topic = *argsIt;
				chanIt->setTopic(topic);
				TopicUpdate(sender, chanIt);
			}
			else
				sendMessage(sender->getFd(), ERR_CHANOPPRIVSNEEDED(channame));
			return ;
		}
		chanIt++;
	}
	sendMessage(sender->getFd(), ERR_NOSUCHCHANNEL(channame));
}
