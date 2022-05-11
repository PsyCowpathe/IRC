/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/05/11 20:43:54 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::grant(std::list<Client>::iterator it, char *buff)
{
	std::list<std::string>	tab;

	if (it->getGranteed() == false)
	{
		if (cutdeBuff(&tab, buff, "PASS") == 1)
		{
			if (tab.begin()->compare(_pass) == 0)
				it->setGranteed(1);
			else
				sendMessage(it->getFd(), RPL_INCORRECTPASS);
		}
		tab.clear();
	}
}

void	Server::nick(std::list<Client>::iterator it, char *buff)
{
	std::list<std::string>	tab;

	if (cutdeBuff(&tab, buff, "NICK") == 1)
	{
		if (tab.size() < 1)
			sendMessage(it->getFd(), RPL_INCORRECTNICK);
		else if (isDuplicate(_client, *tab.begin(), &Client::getNick) == 1)
			sendMessage(it->getFd(), RPL_DUPLICATENICK);
		else
		{
			std::list<std::string>::iterator	itt;
			itt = tab.begin();
			it->setNick(*itt);
			it->setNicked(1);
		}
	}
	tab.clear();
}

void	Server::user(std::list<Client>::iterator it, char *buff)
{
	std::list<std::string>	tab;

	if (cutdeBuff(&tab, buff, "USER") == 1)
	{
		if (tab.size() < 4)
			sendMessage(it->getFd(), RPL_INCORRECTUSER);
		else
		{
			std::list<std::string>::iterator	itt;
			itt = tab.begin();
			it->setUser(*itt);
			itt++;
			itt++;
			itt++;
			it->setReal(*itt);
			it->setUsered(1);
		}
		tab.clear();
	}
}

void	Server::authentication(std::list<Client>::iterator it, char *buff)
{
	if (it->getGranteed() == false)
		grant(it, buff);
	else if (it->getNicked() == false)
		nick(it, buff);
	else if (it->getUsered() == false)
		user(it, buff);
	if (it->getGranteed() == true && it->getNicked() == true && it->getUsered() == true)
	{
		sendMessage(it->getFd(), RPL_WELCOME(it->getUser()));
		/*sendMessage(it->getFd(), RPL_WELCOME(it->getUser()));
		sendMessage(it->getFd(), RPL_WELCOME(it->getUser())); continue greeting
		sendMessage(it->getFd(), RPL_WELCOME(it->getUser()));*/
		it->setRegistered(1);
	}
}