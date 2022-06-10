/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:52:02 by agirona           #+#    #+#             */
/*   Updated: 2022/06/10 19:16:14 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string		Server::downgrade(const char *str)
{
	char			tmp;
	std::string		res;
	int				i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			tmp = str[i] + 32;
		else
			tmp = str[i];
		res = res + tmp;
		i++;
	}
	return (res);
}

void	Server::sendMessage(int fd, std::string msg)
{
	size_t	ret;

	ret = 0;
	while (ret < msg.size())
		ret += send(fd, msg.c_str(), msg.size(), 0);
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

std::list<std::string>		Server::cutTab(std::string str)
{
	size_t					pos;
	std::list<std::string>	res;	
	std::string				tmp;

	pos = str.find(",", 0);
	if (pos == std::string::npos)
		res.push_back(str);
	else
	{
		while (pos != std::string::npos)
		{
			tmp = str.substr(0, pos);
			if (tmp.size() == 1)
				return (res);
			res.push_back(tmp);
			str.erase(0, pos + 1);
			pos = str.find(",", 0);
		}
		if (str.empty() == false)
			res.push_back(str);
	}
	return (res);
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
	if ((ret = str.find(key, 0)) != std::string::npos && ret == 0)
	{
		if (str[ret + key.size()] != ' ' && str[ret + key.size()] != '\0')
			return (0);
		while (pos < str.size())
		{
			pos = str.find_first_not_of(" ", pos);
			point = str.find(":", point);
			if (point == pos && point != std::string::npos)
			{
					tab->push_back(str.substr(pos, str.size() - pos));
					break ;
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
