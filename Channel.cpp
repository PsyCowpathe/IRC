/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:58:20 by agirona           #+#    #+#             */
/*   Updated: 2022/05/30 14:33:58 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
	_topic = "Default topic.";
	//_op.push_back(client);
}

Channel::~Channel()
{
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
	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
			return (1); //deja dans le chan
		it++;
	}
	_user.push_back(client);
	std::cout << "add = " << client.getNick() << std::endl;
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
	if (it == ite)
	{
		_op.push_back(client);
		std::cout << "add = " << client.getNick() << std::endl;
		return (0); //success
	}
	while (it != ite)
	{
		if (it->getNick() == client.getNick())
		{
			_op.push_back(client);
			std::cout << "add = " << client.getNick() << std::endl;
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
	std::list<Client>				all;;
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		std::cout << "op = " << it->getNick() << std::endl;
		all.push_back(*it);
		it++;
	}
	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		std::cout << "user = " << it->getNick() << std::endl;
		all.push_back(*it);
		it++;
	}
	std::cout << "finished" << std::endl;
	return (all);
}

std::string		Channel::getName(void) const
{
	return (_name);
}

int				Channel::setName(const std::string name)
{
	size_t	ret;

	ret = name.find("#", 0);
	if (ret != 0)
		return (1); // # devant le nom
	_name = name;
	return (0); // success;
}

std::string		Channel::getTopic(void) const
{
	return (_topic);
}

int				Channel::setTopic(const std::string topic)
{
	_topic = topic;
	return (0); // success;
}

std::string		Channel::getUserList(void)
{
	std::string						list;
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		list = list + "@" + it->getNick();
		if (++it != ite || _user.empty() == false)
			list = list + " ";
	}
	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		list = list + it->getNick();
		if (++it != ite)
			list = list + " ";
	}
	return (list);
}

int			Channel::isJoin(const std::string &nick)
{
	std::list<Client>			list;
	std::list<Client>::iterator	it;
	std::list<Client>::iterator	ite;

	list = getAllUser();
	it = list.begin();
	ite = list.end();
	while (it != ite)
	{
		if (it->getNick() == nick)
			return (1);
		it++;
	}
	return (0);
}
