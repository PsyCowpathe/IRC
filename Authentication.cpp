/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/05/18 16:30:22 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::authGrant(std::list<Client>::iterator it, const std::string &buff)
{
	std::list<std::string>	tab;

	if (cutdeBuff(&tab, buff, "PASS") == 1)
	{
		if (tab.empty() == true || tab.size() < 1)
			sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("PASS"));
		else if (tab.begin()->compare(_pass) != 0)
			sendMessage(it->getFd(), ERR_PASSWDMISMATCH);
		else
			it->setGranteed(1);
	}
	else
		sendMessage(it->getFd(), ERR_NEEDPASS);
	tab.clear();
}

void	Server::authNick(std::list<Client>::iterator it, const std::string &buff)
{
	std::list<std::string>	tab;
	size_t					ret;
	// CAP LS 302
	ret = -1;

	if (cutdeBuff(&tab, buff, "NICK") == 1)
	{
		std::cout << "tab = " << *tab.begin() << std::endl;
		if (tab.empty() == true || tab.size() < 1)
			sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("NICK"));
		else if (findStr<std::list<Client>, Client>(_client, *tab.begin(), &Client::getNick) != _client.end())
			sendMessage(it->getFd(), ERR_NICKNAMEINUSE(*tab.begin()));
		else if ((ret = tab.begin()->find("#", 0)) == 0 || ret != std::string::npos)
			sendMessage(it->getFd(), ERR_ERRONEUSNICKNAME(*tab.begin()));
		else
		{
			std::list<std::string>::iterator	itt;
			itt = tab.begin();
			it->setNick(*itt);
			it->setNicked(1);
		}
	}
	else
		sendMessage(it->getFd(), ERR_NEEDNICK);
	tab.clear();
}

void	Server::authUser(std::list<Client>::iterator it, const std::string &buff)
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
	}
	else
		sendMessage(it->getFd(), ERR_NEEDUSER);
	tab.clear();
}

void	Server::authentication(std::list<Client>::iterator it, const std::string &buff)
{
	if (it->getGranteed() == false)
		authGrant(it, buff);
	else if (it->getNicked() == false)
		authNick(it, buff);
	else if (it->getUsered() == false)
		authUser(it, buff);
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
