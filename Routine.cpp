/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:13:07 by agirona           #+#    #+#             */
/*   Updated: 2022/06/13 16:24:42 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::newconnection(int *max)
{
	Client	newone;

	newone.setFd(accept(_fd, &newone.getAddr(), &newone.getLen()));
	if (newone.getFd() > *max)
		*max = newone.getFd();
	FD_SET(newone.getFd(), &_master);
	_client.push_back(newone);
	_nbclient++;
	std::cout << "NEW USER HAS JOIN THE SERVER" << std::endl;
}


void	Server::dataReception(int *max, std::list<Client>::iterator it)
{
	int						buff_size = 100;
	char					buff[buff_size + 1];
	int						ret;
	size_t					i;
	std::string				tmp;
	std::list<Client *>::iterator		update;
	std::list<Client *>::iterator		endupdate;
	std::list<Client *>				list;


	ret = 1;
	if (FD_ISSET(it->getFd(), &_watchlist))
	{
		ret = recv(it->getFd(), buff, buff_size, 0);
		if (ret == 0 || ret == -1)
		{
			if (ret == 0)
				std::cout << "CLIENT DISCONNECTED FROM SERVER" << std::endl;
			else
				std::cout << "CONNECTION RESET BY CLIENT" << std::endl;
			std::list<Channel>::iterator	chanit;
			std::list<Channel>::iterator	chanite;

			chanit = _channel.begin();
			chanite = _channel.end();
			while (chanit != chanite)
			{
				if (chanit->deleteUser(&(*it)) == 0)
				{
					list = chanit->getAllUser();
					if (list.size() == 0)
					{
						chanit = _channel.erase(chanit);
						continue ;
					}
					else
					{
						update = list.begin();
						endupdate = list.end();
						while (update != endupdate)
						{
							sendMessage((*update)->getFd(), RPL_EMPTYPART(it->getNick(), chanit->getName()));
							update++;
						}
					}
				}
				chanit++;
			}
			FD_CLR(it->getFd(), &_master);
			close(it->getFd());
			if (it->getFd() == *max)
				*max = newMax();
			_client.erase(it);
			_nbclient--;
		}
		else
		{
			buff[ret] = 0;
			std::cout << "BUFF = " << buff << std::endl;
			it->setBuff(buff);
			while ((i = it->getBuff().find("\r\n", 0)) != std::string::npos)
			{
				tmp = it->getBuff().substr(0, i) + "\0";
				it->eraseBuff(0, i + 2);
				//std::cout << "COMMAND = " << tmp << "|" << std::endl;
				if (it->getRegistered() == false)
					authentication(it, tmp);
				else
					detectCommand(it, tmp);
			}
		}
	}
}

void	Server::routine()
{
	struct timeval					tv;
	int								max;
	int								ret;
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	tv.tv_sec = 2;
	tv.tv_usec = 500000;
	FD_ZERO(&_master);
	FD_ZERO(&_watchlist);
	FD_SET(_fd, &_master);
	max = _fd;
	while (1)
	{
		_watchlist = _master;
		ret = select(max + 1, &_watchlist, NULL, NULL, NULL);
		std::cout << "-------------------------------NewData-----------------------------------" << std::endl;
		if (ret == -1)
			throw SelectException();
		if (FD_ISSET(_fd, &_watchlist))
			newconnection(&max);
		it = _client.begin();
		ite = _client.end();
		while (it != ite)
		{
			dataReception(&max, it++);
		}
	}
}
