/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:11:38 by agirona           #+#    #+#             */
/*   Updated: 2022/06/13 15:42:36 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::structinit()
{
	memset(&_infos, 0, sizeof(_infos));
	_infos.ai_family = AF_INET;
	_infos.ai_socktype = SOCK_STREAM;
	_infos.ai_flags = AI_PASSIVE;
	_infos.ai_protocol = 0;
	_infos.ai_canonname = NULL;
	_infos.ai_addr = NULL;
	_infos.ai_next = NULL;
	if (getaddrinfo(NULL, _port.c_str(), &_infos, &_res) != 0)
	{
		std::cout << strerror(errno) << std::endl;
		throw getaddrException();
	}
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
