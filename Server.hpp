/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:38:28 by agirona           #+#    #+#             */
/*   Updated: 2022/05/05 19:33:50 by agirona          ###   ########lyon.fr   */
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

class	Server
{
	private :
		std::string			_port;
		std::string			_pass;
		int					_fd;
		struct addrinfo		_infos;
		struct addrinfo		*_res;
		int					_nbclient;
		int					_allcount;
		std::list<Client>	_client;
		fd_set          	_master;
		fd_set				_watchlist;


		void				newconnection(int *max);
		void				moov_back(const int b);
		void				dataReception(std::list<Client>::iterator it);

	public :
		Server(std::string port, std::string pass);
		~Server();

		int		getFd() const;
		void	parseport(char *port);
		void	structinit();
		void	serverinit();
		void	routine();

		class	InvalidPortException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
		class	PortFormatException : public std::exception
		{
			public :
				virtual const char	*what() const throw();
		};
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
