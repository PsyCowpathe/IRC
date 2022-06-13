/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:18:16 by agirona           #+#    #+#             */
/*   Updated: 2022/06/13 15:48:34 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::nickUpdate(std::list<std::string> tab, const Client &sender)
{
	std::list<Client>::iterator		clientIt;
	std::list<Client>::iterator		clientIte;

	clientIt = _client.begin();
	clientIte = _client.end();
	while (clientIt != clientIte)
	{
		sendMessage(clientIt->getFd(), RPL_NICK(sender.getNick(), *tab.begin()));
		clientIt++;
	}
}

void	Server::Nick(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	size_t		ret;
	std::string	save;

	std::cout << "NICK" << std::endl;
	if (tab.empty() == true || tab.size() < 1)
	{
		sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("NICK"));
		return ;
	}
	*tab.begin() = downgrade(tab.begin()->c_str());
	if (findStr<std::list<Client>, Client>(_client, *tab.begin(), &Client::getNick) != _client.end())
		sendMessage(sender->getFd(), ERR_NICKNAMEINUSE(*tab.begin()));
	else if ((ret = tab.begin()->find_first_of("@#", 0)) == 0 || ret != std::string::npos)
		sendMessage(sender->getFd(), ERR_ERRONEUSNICKNAME(*tab.begin()));
	else
	{
		nickUpdate(tab, *sender);
		std::cout << "User : " << sender->getNick() << " is now nicknamed ";
		sender->setNick(*tab.begin());
		std::cout << sender->getNick() << std::endl;
	}
}
