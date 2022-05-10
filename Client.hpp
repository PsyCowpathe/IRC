/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:38:59 by agirona           #+#    #+#             */
/*   Updated: 2022/05/10 18:48:13 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef		CLIENT_HPP
# define	CLIENT_HPP

# include <sys/socket.h>
# include <iostream>

class	Client
{
	private :
		int					_fd;
		struct sockaddr		_addr;
		socklen_t			_len;
		bool				_registered;
		bool				_granteed;
		bool				_nicked;
		std::string			_nick;
		bool				_usered;
		std::string			_user;
		std::string			_realname;
	public :
		Client();
		int					getFd() const;
		void				setFd(const int fd);
		bool				getRegistered() const;
		void				setRegistered(const int value);
		bool				getGranteed() const;
		void				setGranteed(const int value);
		bool				getNicked() const;
		void				setNicked(const int value);
		std::string			getNick() const;
		void				setNick(const std::string value);
		void				setUser(const std::string value);
		std::string			getUser() const;
		void				setUsered(const bool value);
		bool				getUsered() const;
		void				setReal(const std::string value);
		std::string			getReal() const;
		struct sockaddr		&getAddr();
		socklen_t			&getLen();
};

#endif
