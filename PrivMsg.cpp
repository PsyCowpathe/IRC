/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:16:45 by agirona           #+#    #+#             */
/*   Updated: 2022/06/09 18:32:58 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::msgAll(std::list<std::string> &args, Client &sender)
{
	std::list<Client *>					list;
	std::list<Client *>::iterator		clientIt;
	std::list<Client *>::iterator		clientIte;
	std::list<Channel>::iterator		chanit;
	std::list<Channel>::iterator		chanite;
	std::list<std::string>::iterator	msg;

	chanit = _channel.begin();
	chanite = _channel.end();
	while (chanit != chanite && chanit->getName() != *args.begin())
		chanit++;
	if (chanit == chanite)
	{
		sendMessage(sender.getFd(), ERR_CANNOTSENDTOCHAN(*args.begin()));
		return ;
	}
	list = chanit->getAllUser();
	clientIt = list.begin();
	clientIte = list.end();
	msg = args.begin();
	msg++;
	if (chanit->isJoin(sender.getNick()) == 0)
	{
		sendMessage(sender.getFd(), ERR_CANNOTSENDTOCHAN(chanit->getName()));
		return ;
	}
	while (clientIt != clientIte)
	{
		if ((*clientIt)->getNick() != sender.getNick())
			sendMessage((*clientIt)->getFd(), RPL_PRIVMSG(sender.getNick(), chanit->getName(), *msg));
		clientIt++;
	}
}

void	Server::privMsg(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	std::list<Client>::iterator			receiver;
	std::string							msg;
	std::list<std::string>				list;
	std::list<std::string>::iterator	listIt;
	std::list<std::string>::iterator	listIte;

	if (tab.size() < 2)
	{
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("PRIVMSG"));
		return ;
	}
	msg = *(++tab.begin());
	list = cutTab(*tab.begin());
	listIt = list.begin();
	listIte = list.end();
	while (listIt != listIte)
	{
		*listIt = downgrade(listIt->c_str());
		receiver = findStr<std::list<Client>, Client>(_client, *listIt, &Client::getNick);
		if (receiver != _client.end() && listIt->find("#", 0) == std::string::npos)
		{
			std::cout << sender->getNick();
			std::cout << " send --> \"" << msg << "\" to " << receiver->getNick() << std::endl;
			sendMessage(receiver->getFd(), RPL_PRIVMSG(sender->getNick(), receiver->getNick(), msg));
		}
		else if (listIt->find("#", 0) != std::string::npos)
			msgAll(tab, *sender);
		else
			sendMessage(sender->getFd(), ERR_NOSUCHNICK(*listIt));
		listIt++;
	}
}
