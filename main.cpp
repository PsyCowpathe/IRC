/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <cguiot@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:30:22 by agirona           #+#    #+#             */
/*   Updated: 2022/05/05 19:33:58 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

void	irc(char *port, char *pass)
{
	Server	server(port, pass);

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
