/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:32:54 by agirona           #+#    #+#             */
/*   Updated: 2022/06/08 16:04:29 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Notice(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	std::list<Client>::iterator			receiver;
	std::string							msg;
	std::list<std::string>				list;
	std::list<std::string>::iterator	listIt;
	std::list<std::string>::iterator	listIte;


	msg = *(++tab.begin());
	list = cutTab(*tab.begin());
	listIt = list.begin();
	listIte = list.end();
	while (listIt != listIte)
	{
		receiver = findStr<std::list<Client>, Client>(_client, *listIt, &Client::getNick);
		if (receiver != _client.end() && listIt->find("#", 0) == std::string::npos)
		{
			std::cout << sender->getNick();
			std::cout << " send --> \"" << msg << "\" to " << receiver->getNick() << std::endl;
			sendMessage(receiver->getFd(), RPL_NOTICE(sender->getNick(), receiver->getNick(), msg));
		}
		else if (listIt->find("#", 0) != std::string::npos)
			msgAll(tab, *sender);
		listIt++;
	}
}
