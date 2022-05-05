/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:54:02 by agirona           #+#    #+#             */
/*   Updated: 2022/05/05 19:33:57 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"

Server::Server(std::string port, std::string pass)
{
	std::cout << "Server created !" << std::endl;
	_port = port;
	_pass = pass;
	_nbclient = 0;
	_allcount = 0;
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

void	Server::newconnection(int *max)
{
	Client	newone;

	newone.setFd(accept(getFd(), &newone.getAddr(), &newone.getLen()));
	if (newone.getFd() > *max)
		*max = newone.getFd();
	newone.setUser(_allcount++);
	FD_SET(newone.getFd(), &_master);
	std::cout << "New connection " << newone.getUser() << std::endl;
	_client.push_back(newone);
	_nbclient++;
}

void	Server::dataReception(std::list<Client>::iterator it)
{
	char			buff[10];
	int				ret;

	bzero(buff, 10);
	ret = 1;
	if (FD_ISSET(it->getFd(), &_watchlist))
	{
		ret = recv(it->getFd(), buff, 10, 0);
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
			std::cout << "User " << it->getUser() << " : " << buff << std::endl;
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
