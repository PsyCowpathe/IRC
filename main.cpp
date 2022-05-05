/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <cguiot@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:30:22 by agirona           #+#    #+#             */
/*   Updated: 2022/05/05 16:57:42 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

void	irc(char *port, char *pass)
{
	Server	server(port, pass);
	Client	client[1000];

	try
	{
		server.parseport(port);
		server.structinit();
		server.serverinit();
		server.routine();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	client[0].setFd(accept(server.getFd(), &client[0].getAddr(), &client[0].getLen()));
	std::cout << "connected" << std::endl;
	/*while (1)
	{
		ret = recv(client[0].getFd(), buff, 10, 0);
		if (ret == 0)
			std::cout << "disconnected" << std::endl;
		else if (ret == -1)
			std::cout << "error" << std::endl;
		else
			std::cout << buff << std::endl;
		bzero(buff, 10);
		std::cout << "new" << std::endl;
	}*/
}

int		main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage : ./IRC <port> <password>" << std::endl;
		return (0);
	}
	else
		irc(argv[1], argv[2]);
}
