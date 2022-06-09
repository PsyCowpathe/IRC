/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 12:31:44 by agirona           #+#    #+#             */
/*   Updated: 2022/06/09 18:32:57 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Invite(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	std::list<std::string>::iterator	argsIt;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client>::iterator		clientIt;
	std::list<Client>::iterator		clientIte;
	std::list<Client>					list;
	std::string							nickname;
	std::string							channame;

	std::cout << "INVITE" << std::endl;
	if (tab.size() < 2)
	{
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("INVITE"));
		return ;
	}
	chanIt = _channel.begin();
	chanIte = _channel.end();
	argsIt = tab.begin();
	nickname = *argsIt++;
	channame = *argsIt;
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == *argsIt)
		{
			if (chanIt->isOp(sender->getNick()) == 0)
			{
				sendMessage(sender->getFd(), ERR_CHANOPPRIVSNEEDED(channame));
				return ;
			}
			if (chanIt->isJoin(nickname) == 1)
			{
				sendMessage(sender->getFd(), ERR_USERONCHANNEL(nickname, channame));
				return ;
			}
			clientIt = _client.begin();
			clientIte = _client.end();
			while (clientIt != clientIte)
			{
				if (clientIt->getNick() == nickname)
				{
					chanIt->addInvite(&(*clientIt));
					sendMessage(sender->getFd(), RPL_INVITING(sender->getNick(), nickname, channame));
					sendMessage(clientIt->getFd(), RPL_INVITED(sender->getNick(), nickname, channame));
					return ;
				}
				clientIt++;
			}
			sendMessage(sender->getFd(), ERR_NOSUCHNICK(nickname));
			return ;
		}
		chanIt++;
	}
	sendMessage(sender->getFd(), ERR_NOSUCHCHANNEL(channame));
}
