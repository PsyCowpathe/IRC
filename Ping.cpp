/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:18:04 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:20:00 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Ping(std::list<std::string> tab, std::list<Client>::iterator it)
{
	(void)tab;
	std::cout << "PONG" << std::endl;
	sendMessage(it->getFd(), RPL_PONG);
}
