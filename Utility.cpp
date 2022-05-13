/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:52:02 by agirona           #+#    #+#             */
/*   Updated: 2022/05/13 20:36:47 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::sendMessage(int fd, std::string msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}

int     Server::newMax()
{
    int                             max;
    std::list<Client>::iterator     it;
    std::list<Client>::iterator     ite;

    max = _fd;
    it = _client.begin();
    ite = _client.end();
    while (it != ite)
    {
        if (it->getFd() > max)
            max = it->getFd();
        it++;
    }
    return (max);
}

std::list<Client>::iterator	Server::findStr(std::list<Client> &lst, std::string str, std::string (Client::*fct)(void) const)
{
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = lst.begin();
	ite = lst.end();
	while (it != ite)
	{
		if (((*it).*fct)() == str)
		{
			std::cout << "J'ai trouver" << std::endl;
			return (it);
		}
		it++;
	}
	std::cout << "J'ai po trouver" << std::endl;
	return (_client.end());
}

int		Server::cutdeBuff(std::list<std::string> *tab, const std::string &buff, const std::string key)
{
	std::string							str;
	size_t								ret;
	size_t								pos;
	size_t								point;

	str = buff;
	pos = key.size();
	point = pos;
	if ((ret = str.find(key, 0)) != std::string::npos)
	{
		while (pos < str.size())
		{
			pos = str.find_first_not_of(" ", pos);
			point = str.find(":", point);
			if (point == pos)
			{
				point = str.find(":", point + 1);
				if (point != std::string::npos)
				{
					tab->push_back(str.substr(pos + 1, point - pos - 1));
					pos = point + 1;
				}
				else
				{
					tab->push_back(str.substr(pos, str.size() - pos));
					break ;
				}
				point++;
			}
			else
			{
				if (pos == std::string::npos)
					break;
				ret = str.find(" ", pos);
				if (ret == std::string::npos)
					ret = str.size();
				tab->push_back(str.substr(pos, ret - pos));
				pos += tab->rbegin()->size();
			}
		}
		return (1);
	}
	return (0);
}
