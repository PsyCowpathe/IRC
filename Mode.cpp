/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:00:45 by agirona           #+#    #+#             */
/*   Updated: 2022/05/30 20:41:58 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// mode <chan> <+o> <pseudo>

void	Server::ModeUpdate(std::list<Client>::iterator &target, const std::list<Channel>::iterator &channel, const std::string &mode)
{
	std::list<Client>					list;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	

	list = channel->getAllUser();
	clientIt = list.begin();
	clientIte = list.end();
	while (clientIt != clientIte)
	{
		sendMessage(clientIt->getFd(), RPL_MODE(target->getNick(), channel->getName(), mode));
		clientIt++;
	}
}

void	Server::Mode(std::list<std::string> tab, std::list<Client>::iterator it) //elle est kc faut la test j'ai la flemme
{
	std::list<std::string>::iterator	argsIt;
	std::list<std::string>::iterator	argsIte;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<Client>					list;
	std::string							mode;

	argsIt = tab.begin();
	argsIte = tab.end();
	chanIt = _channel.begin();
	chanIte = _channel.end();
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == *tab.begin())
		{
			mode = *(++argsIt);
			list = chanIt->getAllUser();
			clientIt = list.begin();
			clientIte = list.end();
			while (clientIt != clientIte)
			{
				if (clientIt->getNick() == *argsIt)
				{
					if (chanIt->isOp(it->getNick()) == 1)
					{
						std::cout << "OPED" << std::endl;
						if (mode == "+o")
						{
							chanIt->addOperator(*clientIt);
							ModeUpdate(clientIt, chanIt, mode);
						}
						else if (mode == "-o")
						{
							chanIt->deleteOperator(*clientIt);
							ModeUpdate(clientIt, chanIt, mode);
						}
						else
							sendMessage(it->getFd(), ERR_UNKNOWMODE(mode));
						return ;
					}
					sendMessage(it->getFd(), ERR_CHANOPPRIVSNEEDED(*tab.begin()));
					return ;
				}
				clientIt++;
			}
			sendMessage(it->getFd(), ERR_NOTONCHANNEL(*tab.begin()));
		}
		chanIt++;
	}
	sendMessage(it->getFd(), ERR_NOSUCHCHANNEL(*tab.begin()));
}
