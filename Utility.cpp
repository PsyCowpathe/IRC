/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:52:02 by agirona           #+#    #+#             */
/*   Updated: 2022/05/18 17:33:53 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
				/*point = str.find(":", point + 1);
				//if (point != std::string::npos)
				//{
				//	tab->push_back(str.substr(pos + 1, point - pos - 1));
				//	pos = point + 1;
				//}
				//else
				{*/
					tab->push_back(str.substr(pos, str.size() - pos));
					break ;
				//}
				//point++;
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
