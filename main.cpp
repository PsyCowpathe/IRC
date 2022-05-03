/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:30:22 by agirona           #+#    #+#             */
/*   Updated: 2022/05/03 21:32:47 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <netdb.h>

#define BACKLOG 10 //combien d'user en attente pour se co au serveur

void	irc(char *port, char *pass)
{
	int				fd;
	int				newfd;
	int				value = 1;
	struct addrinfo infos, *res;
	struct sockaddr newaddr;
	socklen_t 		newaddrlen;
	(void)pass;

	infos.ai_family = AF_INET; //c pour ipv4
    infos.ai_socktype = SOCK_STREAM; // c pour tcp
    infos.ai_flags = AI_PASSIVE; // rempli l'ip tout seul si le premier arg de getaddrrinfo est NULL (ip de l'host)
    infos.ai_protocol = 0; //niveau du socket
    infos.ai_canonname = NULL;
    infos.ai_addr = NULL;
    infos.ai_next = NULL;
	getaddrinfo(NULL, port, &infos, &res);
	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); //cree un socket
	if (fd == -1)
		std::cout << "socket error" << std::endl;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) != 0) //permet de se reconnecter a la meme addresse si on etein le serveur puis on le relance (evite le address already in use)
		std::cout << "socket option error" << std::endl;
	if (bind(fd, res->ai_addr, res->ai_addrlen) != 0) //permet de lier le serveur a un port
		perror("bind()");
	if (listen(fd, BACKLOG) == -1) //dis au serveur d'attentre des connection entrante 
		perror("listen()");
	newfd = accept(fd, &newaddr, &newaddrlen); //accept les connexion recuperer par listen
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
