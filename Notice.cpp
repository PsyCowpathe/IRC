/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 15:32:54 by agirona           #+#    #+#             */
/*   Updated: 2022/06/06 15:40:12 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Notice(std::list<std::string> tab, std::list<Client>::iterator it)
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
		sendMessage(receiver->getFd(), RPL_NOTICE(it->getNick(), receiver->getNick(), *tabit));
	}
	else if (tabit->find("#", 0) != std::string::npos)
		msgAll(tab, *it);
}
