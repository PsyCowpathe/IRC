/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:57:13 by agirona           #+#    #+#             */
/*   Updated: 2022/05/24 22:09:57 by agirona          ###   ########lyon.fr   */
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
		std::list<Client>	_invite;
		std::string			_name;
		std::string			_topic;

	public :
		Channel();
		~Channel();

		int		addUser(const Client &client);
		int		deleteUser(Client &client);
		int		addOperator(const Client &client);
		int		deleteOperator(Client &client);
		std::list<Client>	getOp(void) const;
		std::list<Client>	getUser(void) const;
		std::list<Client>	getAllUser(void);
		std::string			getName(void) const;
		int					setName(const std::string name);
		std::string			getTopic(void) const;
		int					setTopic(const std::string name);
		std::string			getUserList(void);
		int					isJoin(const std::string &nick);
};

#endif
