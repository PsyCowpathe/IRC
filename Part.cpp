/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:50:33 by agirona           #+#    #+#             */
/*   Updated: 2022/05/30 14:33:56 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::Part(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::list<Channel>::iterator		chanIt;
	std::list<Channel>::iterator		chanIte;
	std::list<Client>::iterator			clientIt;
	std::list<Client>::iterator			clientIte;
	std::list<std::string>::iterator	argsIt;
	std::list<std::string>::iterator	argsIte;
	std::list<Client>					userlist;

	std::cout << "PART" << std::endl;
	chanIt = _channel.begin();
	chanIte = _channel.end();
	argsIt = tab.begin();
	argsIte = tab.end();
	while (chanIt != chanIte)
	{
		std::cout << "chan = " << chanIt->getName() << std::endl;
		std::cout << "arg = " << *argsIt << std::endl;
		if (chanIt->getName() == *argsIt)
		{
			if (chanIt->deleteUser(*it) == 0)
			{
				if (argsIt != argsIte)
					sendMessage(it->getFd(), RPL_PART(it->getNick(), chanIt->getName(), argsIt->c_str()));
				else
					sendMessage(it->getFd(), RPL_EMPTYPART(it->getNick(), chanIt->getName()));
				userlist = chanIt->getAllUser();
				if (userlist.size() == 0)
				{
					_channel.erase(chanIt);
					std::cout << "Channel deleted" << std::endl;
				}
				else
				{
					std::cout << "else" << std::endl;
					clientIt = userlist.begin();
					clientIte = userlist.end();
					argsIt++;
					while (clientIt != clientIte)
					{
						std::cout << "update" << std::endl;
						if (argsIt != argsIte)
							sendMessage(clientIt->getFd(), RPL_PART(it->getNick(), chanIt->getName(), argsIt->c_str()));
						else
							sendMessage(clientIt->getFd(), RPL_EMPTYPART(it->getNick(), chanIt->getName()));
						clientIt++;
					}
				}
			}
			else
				;//tes pas dans le chan fdp
		}
		else
			;//chan exist pas
		chanIt++;
	}
}
