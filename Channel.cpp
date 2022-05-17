/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:58:20 by agirona           #+#    #+#             */
/*   Updated: 2022/05/17 20:15:54 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const Client	&client)
{
	std::cout << "New channel created !" << std::endl;
	_op.push_back(client);
}

Channel::~Channel()
{
	std::cout << "Channel destructed !" << std::endl;
}

int		Channel::addUser(const Client &client)
{
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
			return (1); //deja dans le chan
		it++;
	}
	_user.push_back(client);
	return (0); //success
}

int		Channel::deleteUser(Client &client)
{
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
		{
			_user.erase(it);
			return (0); //success
		}
		it++;
	}
	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
		{
			_op.erase(it);
			return (0); //success
		}
		it++;
	}
	return (1); // cant delete user
}

int		Channel::addOperator(const Client &client)
{
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
			return (1); //deja operator
		it++;
	}
	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
		{
			_op.push_back(client);
			return (0); //success
		}
		it++;
	}
	return (1); //n'est pas dans le channel
}

int		Channel::deleteOperator(Client &client)
{
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
		{
			_op.erase(it);
			_user.push_back(client);
			return (0); //success
		}
		it++;
	}
	return (1); //peux pas delete si il y est pas
}

std::list<Client> Channel::getOp(void) const
{
	return (_op);
}

std::list<Client> Channel::getUser(void) const
{
	return (_user);
}

std::list<Client> Channel::getAllUser(void)
{
	std::list<Client>				all;	
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		all.push_back(*it);
		it++;
	}
	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		all.push_back(*it);
		it++;
	}
	return (all);
}
