/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:58:20 by agirona           #+#    #+#             */
/*   Updated: 2022/06/10 19:40:23 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
	_topic = "Default topic.";
	_inviteonly = 0;
	//_op.push_back(client);
}

Channel::~Channel()
{
}

int		Channel::addUser(Client *client)
{
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		if ((*it)->getNick() == client->getNick())
			return (1); //deja dans le chan
		it++;
	}
	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		if ((*it)->getNick() == client->getNick())
			return (1); //deja dans le chan
		it++;
	}
	_user.push_back(client);
	return (0); //success
}

int		Channel::deleteUser(Client *client)
{
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		if ((*it)->getNick() == client->getNick())
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
		if ((*it)->getNick() == client->getNick())
		{
			_op.erase(it);
			return (0); //success
		}
		it++;
	}
	return (1); // cant delete user
}

int		Channel::addOperator(Client *client)
{
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		if ((*it)->getNick() == client->getNick())
			return (1); //deja operator
		it++;
	}
	it = _user.begin();
	ite = _user.end();
	if (it == ite)
	{
		_op.push_back(client);
		return (0); //success
	}
	while (it != ite)
	{
		if ((*it)->getNick() == client->getNick())
		{
			deleteUser(client);
			_op.push_back(client);
			return (0); //success
		}
		it++;
	}
	return (1); //n'est pas dans le channel
}

int		Channel::deleteOperator(Client *client)
{
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		if ((*it)->getNick() == client->getNick())
		{
			_op.erase(it);
			_user.push_back(client);
			return (0); //success
		}
		it++;
	}
	return (1); //peux pas delete si il y est pas
}
			
std::list<Client *> Channel::getOp(void) const
{
	return (_op);
}

std::list<Client *> Channel::getUser(void) const
{
	return (_user);
}

std::list<Client *> Channel::getAllUser(void)
{
	std::list<Client *>					all;
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

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
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

	it = _op.begin();
	ite = _op.end();
	while (it != ite)
	{
		list = list + "@" + (*it)->getNick();
		if (++it != ite || _user.empty() == false)
			list = list + " ";
	}
	it = _user.begin();
	ite = _user.end();
	while (it != ite)
	{
		list = list + (*it)->getNick();
		if (++it != ite)
			list = list + " ";
	}
	return (list);
}

int			Channel::isJoin(const std::string &nick)
{
	std::list<Client *>				list;
	std::list<Client *>::iterator	it;
	std::list<Client *>::iterator	ite;

	list = getAllUser();
	it = list.begin();
	ite = list.end();
	while (it != ite)
	{
		if ((*it)->getNick() == nick)
			return (1);
		it++;
	}
	return (0);
}

int			Channel::isOp(const std::string &nick)
{
	std::list<Client *>				list;
	std::list<Client *>::iterator	it;
	std::list<Client *>::iterator	ite;

	list = getOp();
	it = list.begin();
	ite = list.end();
	while (it != ite)
	{
		if ((*it)->getNick() == nick)
			return (1);
		it++;
	}
	return (0);
}

int			Channel::isInviteOnly() const
{
	return (_inviteonly);
}

void		Channel::setInviteOnly(const int value)
{
	_inviteonly = value;
}

int			Channel::isInvited(const std::string &name)
{
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

	it = _invite.begin();
	ite = _invite.end();
	while (it != ite)
	{
		if ((*it)->getNick() == name)
			return (1);
		it++;
	}
	return (0);
}

void		Channel::addInvite(Client *client)
{
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;

	if (isInvited(client->getNick()) == 0)
		_invite.push_back(client);
	else
	{
		it = _invite.begin();
		ite = _invite.end();
		while (it != ite)
		{
			if ((*it)->getNick() == client->getNick())
			{
				it = _invite.erase(it);
				continue ;
			}
			it++;
		}
	}
}

Client		*Channel::findUser(const std::string &client)
{
	std::list<Client *>::iterator		it;
	std::list<Client *>::iterator		ite;
	std::list<Client *>					list;
	Client								*res;

	list = getAllUser();
	it = list.begin();
	ite = list.end();
	while (it != ite)
	{
		if ((*it)->getNick() == client)
		{
			res = *it;
			return (res);
		}
		it++;
	}
	return (*ite);
}
