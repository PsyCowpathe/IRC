/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:18:04 by agirona           #+#    #+#             */
/*   Updated: 2022/06/07 18:56:41 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Ping(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	(void)tab;
	std::cout << "PONG" << std::endl;
	sendMessage(sender->getFd(), RPL_PONG);
}
