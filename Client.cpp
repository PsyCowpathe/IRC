/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 19:24:34 by agirona           #+#    #+#             */
/*   Updated: 2022/05/09 20:41:54 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	_fd = 0;
	_len = 0;
	_granteed = 0;
}

int		Client::getFd() const
{
	return (_fd);
}

void	Client::setFd(const int fd)
{
	_fd = fd;
}

void	Client::setUser(const int user)
{
	_user = user;
}

int		Client::getUser() const
{
	return (_user);
}


struct sockaddr	&Client::getAddr()
{
	return (_addr);
}

socklen_t	&Client::getLen()
{
	return (_len);
}

bool		Client::getGranteed() const
{
	return (_granteed);
}

void		Client::setGranteed(const int value)
{
	_granteed = value;
}
