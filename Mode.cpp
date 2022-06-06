/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:00:45 by agirona           #+#    #+#             */
/*   Updated: 2022/06/06 14:12:38 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::ModeUpdate(std::list<Client>::iterator &sender, std::list<Client>::iterator &target, const std::list<Channel>::iterator &channel, const std::string &mode, const int method)
{
	std::list<Client>					list;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;


	list = channel->getAllUser();
	clientIt = list.begin();
	clientIte = list.end();
	while (clientIt != clientIte)
	{
		if (method == 1)
			sendMessage(clientIt->getFd(), RPL_MODE(sender->getNick(), target->getNick(), channel->getName(), mode));
		else
			sendMessage(clientIt->getFd(), RPL_MODE(sender->getNick(), "", channel->getName(), mode));
		clientIt++;
	}
}

void	Server::userMode(std::list<std::string> tab, std::list<Client>::iterator &sender)
{
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<std::string>::iterator	argsIt;

	clientIt = _client.begin();
	clientIte = _client.end();
	argsIt = tab.begin();
	while (clientIt != clientIte)
	{
		if (clientIt->getNick() == *argsIt) 
		{
			if (tab.size() == 1)
				sendMessage(sender->getFd(), RPL_UMODEIS(*argsIt, ""));
			else
				sendMessage(sender->getFd(), ERR_UMODEUNKNOWFLAG(*(++argsIt)));
			return ;
		}
		clientIt++;
	}
	sendMessage(sender->getFd(), ERR_NOSUCHNICK(*argsIt));
	return ;
}

void	Server::modeList(std::list<Client>::iterator &sender, std::list<Client>::iterator &target, const std::list<Channel>::iterator &channel, const std::string &mode)
{
	if (mode == "+o")
	{
		std::cout << "OPED" << std::endl;
		channel->addOperator(*target);
		ModeUpdate(sender, target, channel, mode, 1);
	}
	else if (mode == "-o")
	{
		std::cout << "DEOPED" << std::endl;
		channel->deleteOperator(*target);
		ModeUpdate(sender, target, channel, mode, 1);
	}
	else if (mode == "+i")
	{
		std::cout << "Invite only" << std::endl;
		channel->setInviteOnly(1);
		ModeUpdate(sender, target, channel, mode, 0);
	}
	else if (mode == "-i")
	{
		std::cout << "Free join" << std::endl;
		channel->setInviteOnly(0);
		ModeUpdate(sender, target, channel, mode, 0);
	}
	else
		sendMessage(sender->getFd(), ERR_UNKNOWMODE(mode));
}

void	Server::Mode(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::list<std::string>::iterator	argsIt;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<Client>					list;
	std::string							mode;

	if (tab.size() < 1)
	{
		sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("MODE"));
		return ;
	}
	if (!(tab.begin()->find("#", 0) == 0))
	{
		userMode(tab, it);
		return ;
	}
	argsIt = tab.begin();
	chanIt = _channel.begin();
	chanIte = _channel.end();
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == *tab.begin())
		{
			if (tab.size() == 1)
			{
				if (chanIt->isInviteOnly() == 0)
					sendMessage(it->getFd(), RPL_CHANNELMODEIS(*argsIt, "-i"));
				else
					sendMessage(it->getFd(), RPL_CHANNELMODEIS(*argsIt, "+i"));
			}
			else if (tab.size() < 3)
			{
				mode = *(++argsIt);
				clientIt = chanIt->getAllUser().begin();
				modeList(it, clientIt, chanIt, mode);
			}
			else
			{
				mode = *(++argsIt);
				argsIt++;
				list = chanIt->getAllUser();
				clientIt = list.begin();
				clientIte = list.end();
				while (clientIt != clientIte)
				{
					if (clientIt->getNick() == *argsIt)
					{
						if (chanIt->isOp(it->getNick()) == 1)
						{
							modeList(it, clientIt, chanIt, mode);
							return ;
						}
						sendMessage(it->getFd(), ERR_CHANOPPRIVSNEEDED(*tab.begin()));
						return ;
					}
					clientIt++;
				}
				argsIt = tab.begin();
				argsIt++;
				argsIt++;
				sendMessage(it->getFd(), ERR_NOTINCHANNEL(*tab.begin(), *argsIt));
			}
			return ;
		}
		chanIt++;
	}
	sendMessage(it->getFd(), ERR_NOSUCHCHANNEL(*tab.begin()));
}
