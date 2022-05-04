/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:38:59 by agirona           #+#    #+#             */
/*   Updated: 2022/05/04 19:38:26 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef		CLIENT_HPP
# define	CLIENT_HPP

#include <sys/socket.h>

class	Client
{
	private :
		int					_fd;
		struct sockaddr		_addr;
		socklen_t			_len;
	public :
		Client();
		int			getFd() const;
		void		setFd(const int fd);
		struct sockaddr	&getAddr();
		socklen_t	&getLen();
};

#endif
