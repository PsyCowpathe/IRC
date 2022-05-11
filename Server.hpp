/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/05/10 20:43:54 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_HPP
# define	SERVER_HPP

# include "Client.hpp"
# include <iostream>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <list>

# define BACKLOG 10
# define SERVERNAME "God himself"
# define VERSION "1.0"
# define CREATED "10/05/2022 04:20"
# define SERVERNAMEHEAD (static_cast<std::string>(":") + static_cast<std::string>(SERVERNAME))

# define RPL_WELCOME(nickname) (nickname + " 001 " + "Hi " + nickname + ", welcome to this awesome IRC server !" + "\r\n")
# define RPL_YOURHOST(nickname) (nickname + " 002 " + "Your host is " + SERVERNAME + "running version " + VERSION + "\r\n")
# define RPL_CREATED(nickname) (nickname + " 003 " + "This server was created " + CREATED + "\r\n")
# define RPL_MYINFO(nickname) (nickname + " 004 " + nickname + SERVERNAME + VERSION + "none" + "none." + "\r\n")
 
# define RPL_INCORRECTPASS (SERVERNAMEHEAD + " 464 "  "Incorrect password !" + "\r\n")
# define RPL_INCORRECTNICK (SERVERNAMEHEAD + " 464 "  "Incorrect nickname arguments !" + "\r\n")
# define RPL_DUPLICATENICK (SERVERNAMEHEAD + " 433 "  "Error nickname already in use !" + "\r\n")
# define RPL_INCORRECTUSER (SERVERNAMEHEAD + " 464 "  "Incorrect user arguments !" + "\r\n")


class	Server
{
	private :
		std::string			_port;
		std::string			_pass;
		int					_fd;
		struct addrinfo		_infos;
		struct addrinfo		*_res;
		int					_nbclient;
		std::list<Client>	_client;
		fd_set          	_master;
		fd_set				_watchlist;

		void				newconnection(int *max);
		int					newMax();
		void				dataReception(int *max, std::list<Client>::iterator it);
		int					cutdeBuff(std::list<std::string> *tab, const char *buff, const std::string key);
		void				authentication(std::list<Client>::iterator it, char *buff);
		void				sendMessage(int fd, const std::string msg);
		int					isDuplicate(std::list<Client> lst, std::string str, std::string (Client::*fct)(void) const);
		void				grant(std::list<Client>::iterator it, char *buff);
		void				nick(std::list<Client>::iterator it, char *buff);
		void				user(std::list<Client>::iterator it, char *buff);

	public :
		Server(std::string port, std::string pass);
		~Server();

		void				structinit();
		void				serverinit();
		void				routine();

		class	getaddrException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
		class	SocketCreationException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
		class	SocketOptionException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
		class	BindException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
		class	ListenException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
};


#endif
