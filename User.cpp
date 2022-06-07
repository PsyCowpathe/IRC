/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:18:24 by agirona           #+#    #+#             */
/*   Updated: 2022/06/07 18:57:03 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::User(std::list<std::string> tab, std::list<Client>::iterator sender)
{
	(void)tab;
	sendMessage(sender->getFd(), ERR_ALREADYREGISTERED);
}
