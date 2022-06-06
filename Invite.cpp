/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 12:31:44 by agirona           #+#    #+#             */
/*   Updated: 2022/06/06 14:12:36 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Invite(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::list<std::string>::iterator	argsIt;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<Client>					list;
	std::string							nickname;

	std::cout << "INVITE" << std::endl;
	if (tab.size() < 2)
	{
		sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("INVITE"));
		return ;
	}
	chanIt = _channel.begin();
	chanIte = _channel.end();
	argsIt = tab.begin();
	nickname = *argsIt++;
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == *argsIt)
		{
			if (chanIt->isOp(it->getNick()) == 0)
			{
				sendMessage(it->getFd(), ERR_CHANOPPRIVSNEEDED(*(++argsIt)));
				return ;
			}
			if (chanIt->isJoin(nickname) == 1)
			{
				sendMessage(it->getFd(), ERR_USERONCHANNEL(*argsIt, *(++argsIt)));
				return ;
			}
			clientIt = _client.begin();
			clientIte = _client.end();
			while (clientIt != clientIte)
			{
				if (clientIt->getNick() == nickname)
				{
					chanIt->addInvite(*clientIt);
					sendMessage(it->getFd(), RPL_INVITING(*argsIt, *(++argsIt)));
				}
				clientIt++;
			}
			sendMessage(it->getFd(), ERR_NOSUCHNICK(*tab.begin()));
			return ;
		}
		chanIt++;
	}
	sendMessage(it->getFd(), ERR_NOSUCHCHANNEL(*tab.begin()));
}
