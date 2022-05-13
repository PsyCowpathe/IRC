/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/05/13 18:02:43 by agirona          ###   ########lyon.fr   */
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
			if (tab.empty() == true || tab.size() < 1)
				sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("PASS"));
			else if (tab.begin()->compare(_pass) != 0)
				sendMessage(it->getFd(), ERR_PASSWDMISMATCH);
			else
				it->setGranteed(1);
		}
		tab.clear();
	}
}

void	Server::nick(std::list<Client>::iterator it, char *buff)
{
	std::list<std::string>	tab;

	if (cutdeBuff(&tab, buff, "NICK") == 1)
	{
		if (tab.empty() == true || tab.size() < 1)
			sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("NICK"));
		else if (findStr(_client, *tab.begin(), &Client::getNick) != _client.end())
			sendMessage(it->getFd(), ERR_NICKNAMEINUSE(*tab.begin()));
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
		if (tab.empty() == true || tab.size() < 4)
			sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("USER"));
		else
		{
			std::list<std::string>::iterator	itt;
			itt = tab.begin();
			if ((*tab.begin()).size() > PSEUDOLEN)
				(*itt).resize(20);
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
		sendMessage(it->getFd(), RPL_YOURHOST(it->getUser()));
		sendMessage(it->getFd(), RPL_CREATED(it->getUser()));
		sendMessage(it->getFd(), RPL_MYINFO(it->getUser()));
		std::cout << "user = " << it->getNick() << " fd = " << it->getFd() << std::endl;
		it->setRegistered(1);
	}
}
