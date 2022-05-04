/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:54:02 by agirona           #+#    #+#             */
/*   Updated: 2022/05/04 19:38:24 by agirona          ###   ########lyon.fr   */
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
