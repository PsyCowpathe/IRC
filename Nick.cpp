/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:18:16 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:19:57 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Nick(std::list<std::string> tab, std::list<Client>::iterator it)
{
	size_t		ret;

	std::cout << "NICK" << std::endl;
	if (tab.empty() == true || tab.size() < 1)
		sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("NICK"));
	else if (findStr<std::list<Client>, Client>(_client, *tab.begin(), &Client::getNick) != _client.end())
		sendMessage(it->getFd(), ERR_NICKNAMEINUSE(*tab.begin()));
	else if ((ret = tab.begin()->find_first_of("@#", 0)) == 0 || ret != std::string::npos)
		sendMessage(it->getFd(), ERR_ERRONEUSNICKNAME(*tab.begin()));
	else
	{
		std::cout << "User : " << it->getNick() << " is now nicknamed ";
		it->setNick(*tab.begin());
		std::cout << it->getNick() << std::endl;
	}
}
