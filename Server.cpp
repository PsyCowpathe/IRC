/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:54:02 by agirona           #+#    #+#             */
/*   Updated: 2022/05/10 18:48:15 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"

Server::Server(std::string port, std::string pass)
{
	std::cout << "Server created !" << std::endl;
	_port = port;
	_pass = pass;
	_nbclient = 0;
}

Server::~Server()
{
	std::cout << "Server destructed !" << std::endl;
}

int		Server::getFd() const
{
	return (_fd);
}

void	Server::parseport(char *port)
{			
	std::string tmp = port;
	int		nb;

	if (tmp.empty() == true)
		throw PortFormatException();
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (!(isdigit(port[i])))
			throw PortFormatException();
	}
	nb = atoi(port);
	if (nb < 0 || nb > 65535) 
		throw InvalidPortException();
}

void	Server::structinit()
{
	_infos.ai_family = AF_INET;
	_infos.ai_socktype = SOCK_STREAM;
	_infos.ai_flags = AI_PASSIVE;
	_infos.ai_protocol = 0;
	_infos.ai_canonname = NULL;
	_infos.ai_addr = NULL;
	_infos.ai_next = NULL;
	if (getaddrinfo(NULL, _port.c_str(), &_infos, &_res) != 0)
		throw getaddrException();
}

void	Server::serverinit()
{
	int		value;

	value = 1;
	if ((_fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol)) == -1)
		throw SocketCreationException();
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw SocketOptionException();
	if (bind(_fd, _res->ai_addr, _res->ai_addrlen) == -1)
		throw BindException();
	if (listen(_fd, BACKLOG) == -1)
		throw ListenException();
	std::cout << "Now listening on port : " + _port << std::endl;
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

void	Server::newconnection(int *max)
{
	Client	newone;

	newone.setFd(accept(getFd(), &newone.getAddr(), &newone.getLen()));
	if (newone.getFd() > *max)
		*max = newone.getFd();
	FD_SET(newone.getFd(), &_master);
	_client.push_back(newone);
	_nbclient++;
}

void	Server::authentication(std::list<Client>::iterator it, char *buff)
{
	std::list<std::string>	tab;

	if (it->getGranteed() == false)
	{
		if (cutdeBuff(&tab, buff, "PASS") == 1)
		{
			if (tab.begin()->compare(_pass) == 0)
			{
				it->setGranteed(1);
			}
			else
				;//refuse connection
		}
		tab.clear();
	}
	else if (it->getNicked() == false)
	{
		if (cutdeBuff(&tab, buff, "NICK") == 1)
		{
			std::list<std::string>::iterator	itt;
			itt = tab.begin();
			it->setNick(*itt);
			it->setNicked(1);
		}
		tab.clear();
	}
	else if (it->getUsered() == false)
	{
		if (cutdeBuff(&tab, buff, "USER") == 1)
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
	if (it->getGranteed() == true && it->getNicked() == true && it->getUsered() == true)
	{
		//:server 001 <nick> :Welcome to the <network> Network, <nick>
		char	welcome[] = ":SERVER 001 agirona:Welcome to the NTM Network, agirona[!agirona@127.0.0.1]\r\n";
		send(it->getFd(), welcome, strlen(welcome), 0);

		char	welcome1[] = ":SERVER 002 agirona :Your host is NTM2, running version 10.10.10\r\n";
		send(it->getFd(), welcome1, strlen(welcome1), 0);

		char	welcome2[] = ":SERVER 003 agirona :This server was created 15/07/1998\r\n";
		send(it->getFd(), welcome2, strlen(welcome2), 0);

		char	welcome3[] =  ":SERVER 004 agirona NTM2 10.10.10 o + [<channel modes with a parameter>]\r\n";
		send(it->getFd(), welcome3, strlen(welcome3), 0);
		it->setRegistered(1);
	}
}

void	Server::dataReception(std::list<Client>::iterator it)
{
	int						buff_size = 100;
	char					buff[buff_size];
	int						ret;

	bzero(buff, buff_size);
	ret = 1;
	if (FD_ISSET(it->getFd(), &_watchlist))
	{
		ret = recv(it->getFd(), buff, buff_size, 0);
		if (ret == 0)
		{
			std::cout << "Client disconnected !" << std::endl;
			FD_CLR(it->getFd(), &_master);
			_client.erase(it);
			_nbclient--;
		}
		else if (ret == -1)
			std::cout << "Error" << std::endl;
		else
		{
			if (buff[ret - 2] == 13)
				buff[ret - 2] = '\0';
			if (ret == buff_size)
				buff[ret] = '\0';
			else
				buff[ret - 1] = '\0';
			std::cout << "Buff = " << buff << std::endl;
			if (it->getRegistered() == false)
				authentication(it, buff);
		}
	}
}

void	Server::routine()
{
	struct timeval	tv;
	int				max;
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	tv.tv_sec = 2;
	tv.tv_usec = 500000;
	FD_ZERO(&_master);
	FD_ZERO(&_watchlist);
	FD_SET(getFd(), &_master);
	max = getFd();
	while (1)
	{
		_watchlist = _master;
		select(max + 1, &_watchlist, NULL, NULL, NULL);
		if (FD_ISSET(getFd(), &_watchlist))
			newconnection(&max);
		it = _client.begin();
		ite = _client.end();
		while (it != ite)
			dataReception(it++);
	}
}

const char	*Server::InvalidPortException::what() const throw()
{
	return ("The port must be between 0 and 65535 !");
}

const char	*Server::PortFormatException::what() const throw()
{
	return ("The port can't be empty and must be composed of positive digits !");
}

const char	*Server::getaddrException::what() const throw()
{
	return ("Error while geting the address info !");
}

const char	*Server::SocketCreationException::what() const throw()
{
	return ("Error while creating the server's socket !");
}

const char	*Server::SocketOptionException::what() const throw()
{
	return ("Error while setting the socket's option !");
}

const char	*Server::BindException::what() const throw()
{
	return ("Error while binding the server on a port !");
}

const char	*Server::ListenException::what() const throw()
{
	return ("Error while listening !");
}
