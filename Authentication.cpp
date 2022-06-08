/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/06/08 19:41:26 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::authGrant(std::list<Client>::iterator sender, const std::string &buff)
{
	std::list<std::string>	tab;

	if (cutdeBuff(&tab, buff, "PASS") == 1)
	{
		if (tab.empty() == true || tab.size() < 1)
			sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("PASS"));
		else if (tab.begin()->compare(_pass) != 0)
			sendMessage(sender->getFd(), ERR_PASSWDMISMATCH);
		else
			sender->setGranteed(1);
	}
	tab.clear();
}

void	Server::authNick(std::list<Client>::iterator sender, const std::string &buff)
{
	std::list<std::string>	tab;
	size_t					ret;

	ret = -1;
	if (cutdeBuff(&tab, buff, "NICK") == 1)
	{
		if (tab.empty() == true || tab.size() < 1)
			sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("NICK"));
		else if (findStr<std::list<Client>, Client>(_client, *tab.begin(), &Client::getNick) != _client.end())
			sendMessage(sender->getFd(), ERR_NICKNAMEINUSE(*tab.begin()));
		else if ((ret = tab.begin()->find_first_of("@#", 0)) == 0 || ret != std::string::npos)
			sendMessage(sender->getFd(), ERR_ERRONEUSNICKNAME(*tab.begin()));
		else
		{
			std::list<std::string>::iterator	itt;
			itt = tab.begin();
			sender->setNick(*itt);
			sender->setNicked(1);
		}
	}
	tab.clear();
}

void	Server::authUser(std::list<Client>::iterator sender, const std::string &buff)
{
	std::list<std::string>	tab;

	if (cutdeBuff(&tab, buff, "USER") == 1)
	{
		if (tab.empty() == true || tab.size() < 4)
			sendMessage(sender->getFd(), ERR_NEEDMOREPARAMS("USER"));
		else
		{
			std::list<std::string>::iterator	itt;
			itt = tab.begin();
			if ((*tab.begin()).size() > PSEUDOLEN)
				(*itt).resize(20);
			sender->setUser(*itt);
			itt++;
			itt++;
			itt++;
			sender->setReal(*itt);
			sender->setUsered(1);
		}
	}
	tab.clear();
}

void	Server::authentication(std::list<Client>::iterator sender, const std::string &buff)
{
	if (sender->getGranteed() == false)
		authGrant(sender, buff);
	if (sender->getGranteed() == true &&  sender->getNicked() == false)
		authNick(sender, buff);
	if (sender->getGranteed() == true && sender->getUsered() == false)
		authUser(sender, buff);
	if (sender->getGranteed() == true && sender->getNicked() == true && sender->getUsered() == true)
	{
		sendMessage(sender->getFd(), RPL_WELCOME(sender->getNick()));
		sendMessage(sender->getFd(), RPL_YOURHOST(sender->getUser()));
		sendMessage(sender->getFd(), RPL_CREATED(sender->getUser()));
		sendMessage(sender->getFd(), RPL_MYINFO(sender->getUser()));
		std::cout << "user = " << sender->getNick() << " fd = " << sender->getFd() << std::endl;
		sender->setRegistered(1);
	}
}
