/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Excepton.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 02:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/05/11 20:43:54 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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