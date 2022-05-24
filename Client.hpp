/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:38:59 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:09:58 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef		CLIENT_HPP
# define	CLIENT_HPP

# include <sys/socket.h>
# include <iostream>
# include <string.h>

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
		std::string			_buff;
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
		void				setBuff(const std::string value);
		std::string			getBuff() const;
		void				eraseBuff(int start, int len);
		struct sockaddr		&getAddr();
		socklen_t			&getLen();
};

#endif
