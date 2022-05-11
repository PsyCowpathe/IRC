/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <cguiot@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:30:22 by agirona           #+#    #+#             */
/*   Updated: 2022/05/09 20:41:55 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

void	parseport(char *port)
{			
	std::string tmp = port;
	int		nb;

	if (tmp.empty() == true)
		std::cout << "The port can't be empty and must be composed of positive digits !" << std::endl;
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (!(isdigit(port[i])))
			std::cout << "The port can't be empty and must be composed of positive digits !" << std::endl;
	}
	nb = atoi(port);
	if (nb < 0 || nb > 65535)
	std::cout << "The port must be between 0 and 65535 !" << std::endl;
}

void	irc(char *port, char *pass)
{
	Server	server(port, pass);

	try
	{
		parseport(port);
		server.structinit();
		server.serverinit();
		server.routine();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
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
