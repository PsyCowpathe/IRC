/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 19:24:34 by agirona           #+#    #+#             */
/*   Updated: 2022/05/04 19:38:25 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	_fd = 0;
	_len = 0;
}

int		Client::getFd() const
{
	return (_fd);
}

void	Client::setFd(const int fd)
{
	_fd = fd;
}

struct sockaddr	&Client::getAddr()
{
	return (_addr);
}

socklen_t	&Client::getLen()
{
	return (_len);
}
