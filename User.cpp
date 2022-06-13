/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:18:24 by agirona           #+#    #+#             */
/*   Updated: 2022/06/13 15:53:34 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::User(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	(void)tab;
	std::cout << "USER || PASS" << std::endl;
	sendMessage(sender->getFd(), ERR_ALREADYREGISTERED);
}
