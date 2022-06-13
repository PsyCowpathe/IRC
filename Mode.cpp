/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:00:45 by agirona           #+#    #+#             */
/*   Updated: 2022/06/13 15:49:16 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::ModeUpdate(std::list<Client>::iterator &sender, Client *target, const std::list<Channel>::iterator &channel, const std::string &mode, const int method)
{
	std::list<Client *>					list;
	std::list<Client *>::iterator		clientIt;
	std::list<Client *>::iterator		clientIte;


	list = channel->getAllUser();
	clientIt = list.begin();
	clientIte = list.end();
	while (clientIt != clientIte)
	{
		if (method == 1)
			sendMessage((*clientIt)->getFd(), RPL_MODE(sender->getNick(), target->getNick(), channel->getName(), mode));
		else
			sendMessage((*clientIt)->getFd(), RPL_MODE(sender->getNick(), "", channel->getName(), mode));
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

void	Server::modeList(std::list<Client>::iterator &sender, Client *target, const std::list<Channel>::iterator &channel, const std::string &mode)
{
	if (mode == "+o")
	{
		channel->addOperator(target);
		ModeUpdate(sender, target, channel, mode, 1);
	}
	else if (mode == "-o")
	{
		channel->deleteOperator(target);
		ModeUpdate(sender, target, channel, mode, 1);
	}
	else if (mode == "+i")
	{
		channel->setInviteOnly(1);
		ModeUpdate(sender, target, channel, mode, 0);
	}
	else if (mode == "-i")
	{
		channel->setInviteOnly(0);
		ModeUpdate(sender, target, channel, mode, 0);
	}
	else
		sendMessage(sender->getFd(), ERR_UNKNOWMODE(mode));
}

void	Server::Mode(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	std::list<std::string>::iterator	argsIt;
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client *>::iterator		clientIt;
	std::list<Client *>::iterator		clientIte;
	std::list<Client *>					list;
	std::string							mode;

	std::cout << "MODE" << std::endl;
	if (tab.size() < 1)
	{
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("MODE"));
		return ;
	}
	if (!(tab.begin()->find("#", 0) == 0))
	{
		userMode(tab, sender);
		return ;
	}
	argsIt = tab.begin();
	chanIt = _channel.begin();
	chanIte = _channel.end();
	*tab.begin() = downgrade(tab.begin()->c_str());
	while (chanIt != chanIte)
	{
		if (chanIt->getName() == *tab.begin())
		{
			if (tab.size() == 1)
			{
				if (chanIt->isInviteOnly() == 0)
					sendMessage(sender->getFd(), RPL_CHANNELMODEIS(*argsIt, "-i"));
				else
					sendMessage(sender->getFd(), RPL_CHANNELMODEIS(*argsIt, "+i"));
			}
			else if (tab.size() < 3)
			{
				if (chanIt->isOp(sender->getNick()) == 0)
					sendMessage(sender->getFd(), ERR_CHANOPPRIVSNEEDED(*tab.begin()));
				else
				{
					mode = *(++argsIt);
					list = chanIt->getAllUser();
					clientIt = list.begin();
					modeList(sender, *clientIt, chanIt, mode);
				}
			}
			else
			{
				mode = *(++argsIt);
				argsIt++;
				*argsIt = downgrade(argsIt->c_str());
				list = chanIt->getAllUser();
				clientIt = list.begin();
				clientIte = list.end();
				while (clientIt != clientIte)
				{
					if ((*clientIt)->getNick() == *argsIt)
					{
						if (chanIt->isOp(sender->getNick()) == 1)
						{
							modeList(sender, *clientIt, chanIt, mode);
							return ;
						}
						sendMessage(sender->getFd(), ERR_CHANOPPRIVSNEEDED(*tab.begin()));
						return ;
					}
					clientIt++;
				}
				argsIt = tab.begin();
				argsIt++;
				argsIt++;
				sendMessage(sender->getFd(), ERR_NOTINCHANNEL(*tab.begin(), *argsIt));
			}
			return ;
		}
		chanIt++;
	}
	sendMessage(sender->getFd(), ERR_NOSUCHCHANNEL(*tab.begin()));
}
