/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 19:24:34 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:09:53 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client()
{
	_fd = 0;
	_len = 0;
	_granteed = 0;
	_nicked = 0;
	_registered = 0;
	_usered = 0;
}

int				Client::getFd() const
{
	return (_fd);
}

void			Client::setFd(const int fd)
{
	_fd = fd;
}

struct sockaddr	&Client::getAddr()
{
	return (_addr);
}

socklen_t		&Client::getLen()
{
	return (_len);
}

bool			Client::getRegistered() const
{
	return (_registered);
}

void			Client::setRegistered(const int value)
{
	_registered = value;
}

bool			Client::getGranteed() const
{
	return (_granteed);
}

void			Client::setGranteed(const int value)
{
	_granteed = value;
}

bool			Client::getNicked() const
{
	return (_nicked);
}

void			Client::setNicked(const int value)
{
	_nicked = value;
}

std::string		Client::getNick() const
{
	return (_nick);
}

void			Client::setNick(const std::string value)
{
	_nick = value;
}

void			Client::setUsered(const bool value)
{
	_usered = value;
}

bool			Client::getUsered() const
{
	return (_usered);
}

void			Client::setUser(const std::string value)
{
	_user = value;
}

std::string		Client::getUser() const
{
	return (_user);
}

void			Client::setReal(const std::string value)
{
	_realname = value;
}

std::string		Client::getReal() const
{
	return (_realname);
}

void			Client::setBuff(const std::string value)
{
	_buff = _buff + value;
}

std::string		Client::getBuff() const
{
	return (_buff);
}

void			Client::eraseBuff(int start, int len)
{
	_buff.erase(start, len);
}
