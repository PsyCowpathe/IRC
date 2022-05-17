/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:57:13 by agirona           #+#    #+#             */
/*   Updated: 2022/05/17 20:15:51 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef		Channel_HPP
# define	Channel_HPP

# include <iostream>
# include <list>
# include "Client.hpp"

class	Channel
{
	private :
		std::list<Client>	_user;
		std::list<Client>	_op;

	public :
		Channel(const Client &client);
		~Channel();

		int		addUser(const Client &client);
		int		deleteUser(Client &client);
		int		addOperator(const Client &client);
		int		deleteOperator(Client &client);
		std::list<Client>	getOp(void) const;
		std::list<Client>	getUser(void) const;
		std::list<Client>	getAllUser(void);
};

#endif
