/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/05/17 18:16:23 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_HPP
# define	SERVER_HPP

# include "Client.hpp"
# include "Channel.hpp"
# include <iostream>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <list>

# define BACKLOG 10
# define PSEUDOLEN 20
# define SERVERNAME "God_himself"
# define VERSION "1.0"
# define CREATED "10/05/2022 04:20"
# define SERVERNAMEHEAD (static_cast<std::string>(":") + static_cast<std::string>(SERVERNAME))

# define RPL_WELCOME(nickname) (SERVERNAMEHEAD + " 001 " + nickname + " Hi ! Welcome to this awesome IRC server !" + "\r\n")
# define RPL_YOURHOST(nickname) (SERVERNAMEHEAD + " 002 " + "Your host is " + SERVERNAME + " running version " + VERSION + "\r\n")
# define RPL_CREATED(nickname) (SERVERNAMEHEAD + " 003 " + "This server was created " + CREATED + "\r\n")
# define RPL_MYINFO(nickname) (SERVERNAMEHEAD + " 004 " + nickname + " " + SERVERNAME + " " + VERSION + " none " + "none." + "\r\n")
 
# define ERR_PASSWDMISMATCH (SERVERNAMEHEAD + " 464 " + "Incorrect password !" + "\r\n")
# define ERR_NEEDMOREPARAMS(command) (SERVERNAMEHEAD + " 461 " + command " :Not enough parameters" + "\r\n")
# define ERR_NICKNAMEINUSE(nick) (SERVERNAMEHEAD + " 433 " + nick + " :Nickname is already in use" + "\r\n")
# define ERR_NOSUCHNICK(nick) (SERVERNAMEHEAD + " 401 " + nick + " :No such nick/channel" + "\r\n")
# define ERR_NEEDPASS (SERVERNAMEHEAD + " 4001 " + "Please enter server password !" + "\r\n")
# define ERR_NEEDNICK (SERVERNAMEHEAD + " 4002 " + "Please nick yourself !" + "\r\n")
# define ERR_NEEDUSER (SERVERNAMEHEAD + " 4003 " + "Please give more info about you !" + "\r\n")
# define ERR_INVALIDCOMMAND (SERVERNAMEHEAD + " 4004 " + "Command not found !" + "\r\n")

# define RPL_PRIVMSG(sender, receiver, msg) (":" + sender + " PRIVMSG " + receiver + " " + msg + "\r\n")
# define RPL_PONG (SERVERNAMEHEAD + " PONG " + SERVERNAME + " " + SERVERNAMEHEAD + "\r\n")


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
		std::list<Channel>	_chan;
		fd_set          	_master;
		fd_set				_watchlist;
		int					_nbcommand;

		static std::string	commandList[];
		static void			(Server::*commandFct[])(std::list<std::string> tab, std::list<Client>::iterator it);

		void				Join(std::list<std::string> tab, std::list<Client>::iterator it);
		void				privMsg(std::list<std::string> tab, std::list<Client>::iterator it);
		void				Ping(std::list<std::string> tab, std::list<Client>::iterator it);
		void				Nick(std::list<std::string> tab, std::list<Client>::iterator it);

		void				newconnection(int *max);
		int					newMax();
		void				dataReception(int *max, std::list<Client>::iterator it);
		int					cutdeBuff(std::list<std::string> *tab, const std::string &buff, const std::string key);
		void				authentication(std::list<Client>::iterator it, const std::string &buff);
		void				sendMessage(int fd, const std::string msg);
		std::list<Client>::iterator		findStr(std::list<Client> &lst, std::string str, std::string (Client::*fct)(void) const);
		void				authGrant(std::list<Client>::iterator it, const std::string &buff);
		void				authNick(std::list<Client>::iterator it, const std::string &buff);
		void				authUser(std::list<Client>::iterator it, const std::string &buff);
		void				detectCommand(std::list<Client>::iterator it, const std::string &buff);

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
		class	SelectException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
};


#endif
