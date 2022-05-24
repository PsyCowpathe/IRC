/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:16:45 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:17:24 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::msgAll(std::list<std::string> &args, Client &sender)
{
	std::list<Client>					list;
	std::list<Client>::iterator			it;
	std::list<Client>::iterator			ite;
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
	it = list.begin();
	ite = list.end();
	msg = args.begin();
	msg++;
	if (chanit->isJoin(sender.getNick()) == 0)
	{
		sendMessage(sender.getFd(), ERR_CANNOTSENDTOCHAN(chanit->getName()));
		return ;
	}
	while (it != ite)
	{
		if (it->getNick() != sender.getNick())
			sendMessage(it->getFd(), RPL_PRIVMSG(sender.getNick(), chanit->getName(), *msg));
		it++;
	}
}

void	Server::privMsg(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::list<Client>::iterator			receiver;
	std::list<std::string>::iterator	tabit;
	tabit = tab.begin();
	receiver = findStr<std::list<Client>, Client>(_client, *tabit, &Client::getNick);
	if (receiver != _client.end() && tabit->find("#", 0) == std::string::npos)
	{
		tabit++;
		std::cout << it->getNick();
		std::cout << " send --> \"" << *tabit << "\" to " << receiver->getNick() << std::endl;
		sendMessage(receiver->getFd(), RPL_PRIVMSG(it->getNick(), receiver->getNick(), *tabit));
	}
	else if (tabit->find("#", 0) != std::string::npos)
		msgAll(tab, *it);
	else
		sendMessage(it->getFd(), ERR_NOSUCHNICK(*tabit));
}
