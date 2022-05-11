/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:52:02 by agirona           #+#    #+#             */
/*   Updated: 2022/05/11 20:43:56 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::sendMessage(int fd, std::string msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}

int		Server::isDuplicate(std::list<Client> lst, std::string str, std::string (Client::*fct)(void) const)
{
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = lst.begin();
	ite = lst.end();
	while (it != ite)
	{
		if (((*it).*fct)() == str)
			return (1);
		it++;
	}
	return (0);
}

int		Server::cutdeBuff(std::list<std::string> *tab, const char *buff, const std::string key)
{
	std::string							str;
	size_t								ret;
	size_t								pos;

	str = buff;
	pos = key.size();
	if ((ret = str.find(key, 0)) != std::string::npos)
	{
		while (pos < str.size())
		{
			pos = str.find_first_not_of(" ", pos);
			ret = str.find(" ", pos);
			if (ret == std::string::npos)
				ret = str.size();
			tab->push_back(str.substr(pos, ret - pos));
			pos += tab->rbegin()->size();
		}
		return (1);
	}
	return (0);
}

