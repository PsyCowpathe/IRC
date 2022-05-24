/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 22:18:24 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:19:59 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::User(std::list<std::string> tab, std::list<Client>::iterator it)
{
	(void)tab;
	sendMessage(it->getFd(), ERR_ALREADYREGISTERED);
}
