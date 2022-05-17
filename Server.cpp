/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 17:54:02 by agirona           #+#    #+#             */
/*   Updated: 2022/05/17 18:16:20 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"

Server::Server(std::string port, std::string pass)
{
	std::cout << "Server created !" << std::endl;

	_port = port;
	_pass = pass;
	_nbclient = 0;
	_nbcommand = 4;
}

Server::~Server()
{
	std::cout << "Server destructed !" << std::endl;
}

void	Server::structinit()
{
	memset(&_infos, 0, sizeof(_infos)); //for windows
	_infos.ai_family = AF_INET;
	_infos.ai_socktype = SOCK_STREAM;
	_infos.ai_flags = AI_PASSIVE;
	_infos.ai_protocol = 0;
	_infos.ai_canonname = NULL;
	_infos.ai_addr = NULL;
	_infos.ai_next = NULL;
	if (getaddrinfo("127.0.0.1", _port.c_str(), &_infos, &_res) != 0)
	{
		std::cout << strerror(errno) << std::endl;
		throw getaddrException();
	}
}

void	Server::serverinit()
{
	int		value;

	value = 1;
	if ((_fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol)) == -1)
		throw SocketCreationException();
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw SocketOptionException();
	if (bind(_fd, _res->ai_addr, _res->ai_addrlen) == -1)
		throw BindException();
	if (listen(_fd, BACKLOG) == -1)
		throw ListenException();
	std::cout << "Now listening on port : " + _port << std::endl;
}

void	Server::newconnection(int *max)
{
	Client	newone;

	newone.setFd(accept(_fd, &newone.getAddr(), &newone.getLen()));
	if (newone.getFd() > *max)
		*max = newone.getFd();
	FD_SET(newone.getFd(), &_master);
	_client.push_back(newone);
	_nbclient++;
}


void	Server::dataReception(int *max, std::list<Client>::iterator it)
{
	int						buff_size = 100;
	char					buff[buff_size];
	int						ret;
	size_t					i;
	std::string				tmp;

	bzero(buff, buff_size);
	ret = 1;
	if (FD_ISSET(it->getFd(), &_watchlist))
	{
		ret = recv(it->getFd(), buff, buff_size, 0);
		if (ret == 0 || ret == -1)
		{
			if (ret == 0)
				std::cout << "Client disconnected !" << std::endl;
			else
				std::cout << "Connection reset by client !" << std::endl;
			FD_CLR(it->getFd(), &_master);
			if (it->getFd() == *max)
				*max = newMax();
			_client.erase(it);
			_nbclient--;
		}
		else
		{
			it->setBuff(buff);
			std::cout << "buff = " << buff << std::endl;
			if ((i = it->getBuff().find("\r\n", 0)) != std::string::npos)
			{
				tmp = it->getBuff().substr(0, i) + "\0";
				it->eraseBuff(0, i + 2);
				std::cout << "command = " << tmp << "|" << std::endl;
				if (it->getRegistered() == false)
					authentication(it, tmp);
				else
					detectCommand(it, tmp);
			}
		}
	}
}

void	Server::routine()
{
	struct timeval					tv;
	int								max;
	int								ret;
	std::list<Client>::iterator		it;
	std::list<Client>::iterator		ite;

	tv.tv_sec = 2;
	tv.tv_usec = 500000;
	FD_ZERO(&_master);
	FD_ZERO(&_watchlist);
	FD_SET(_fd, &_master);
	max = _fd;
	while (1)
	{
		_watchlist = _master;
		ret = select(max + 1, &_watchlist, NULL, NULL, NULL);
		if (ret == -1)
			throw SelectException();
		if (FD_ISSET(_fd, &_watchlist))
			newconnection(&max);
		it = _client.begin();
		ite = _client.end();
		while (it != ite)
			dataReception(&max, it++);
	}
}

void	Server::detectCommand(std::list<Client>::iterator it, const std::string &buff)
{
	int		i;
	int		valid;
	std::list<std::string>	tab;

	i = 0;
	valid = 0;
	while (i < _nbcommand)
	{
		if (cutdeBuff(&tab, buff, commandList[i]) == 1)
		{
			valid = 1;
			(this->*commandFct[i])(tab, it);
		}
		i++;
	}
	if (valid == 0)
		sendMessage(it->getFd(), ERR_INVALIDCOMMAND);
}

std::string     Server::commandList[] =
{
	"JOIN",
	"PRIVMSG",
	"PING",
	"NICK",
};

void    (Server::*(Server::commandFct)[])(std::list<std::string> tab, std::list<Client>::iterator it) =
{
	&Server::Join,
	&Server::privMsg,
	&Server::Ping,
	&Server::Nick,
};

void	Server::Join(std::list<std::string> tab, std::list<Client>::iterator it)
{
	(void)it;
	(void)tab;
	std::cout << "JOIN" << std::endl;
}

void	Server::privMsg(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::list<Client>::iterator			receiver;
	std::list<std::string>::iterator	tabit;
	tabit = tab.begin();
	receiver = findStr(_client, *tabit, &Client::getNick);
	if (receiver == _client.end())
		sendMessage(it->getFd(), ERR_NOSUCHNICK(*tabit));
	else
	{
		tabit++;
		std::cout << it->getNick();
		std::cout << " send --> \"" << *tabit << "\" to " << receiver->getNick() << std::endl;
		sendMessage(receiver->getFd(), RPL_PRIVMSG(it->getNick(), receiver->getNick(), *tabit));
	}

}

void	Server::Ping(std::list<std::string> tab, std::list<Client>::iterator it)
{
	(void)tab;
	std::cout << "PONG" << std::endl;
	sendMessage(it->getFd(), RPL_PONG);
}

void	Server::Nick(std::list<std::string> tab, std::list<Client>::iterator it)
{
	std::cout << "NICK" << std::endl;
	if (tab.empty() == true || tab.size() < 1)
		sendMessage(it->getFd(), ERR_NEEDMOREPARAMS("NICK"));
	else if (findStr(_client, *tab.begin(), &Client::getNick) != _client.end())
		sendMessage(it->getFd(), ERR_NICKNAMEINUSE(*tab.begin()));
	else
	{
		std::cout << "User : " << it->getNick() << " is now called ";
		it->setNick(*tab.begin());
		std::cout << it->getNick() << std::endl;
	}
}
