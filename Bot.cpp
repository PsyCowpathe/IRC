/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 17:24:34 by agirona           #+#    #+#             */
/*   Updated: 2022/06/13 16:15:50 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string     Server::bot_find[] =
{
	"oui",
	"non",
	"quoi",
	"comment",
	"palestine",
	"israel",
	"sa va",
	"sava",
	"ptn",
	"merde",
	"connard",
	"pain au chocolat",
	"chocolatine",
	"c'est pas faux",	
	"graille",
};

std::string     Server::bot_answer[] =
{
	"stiti",
	"bril",
	"feur",
	"dans cousteau",
	"Mais non ! Israel !",
	"Mais non ! Palestine !",
	"Apran a aicrir fdp",
	"Apran a aicrir fdp",
	"Avec un language pareil ta maman aurait mieux fait de t'avaler",
	"Avec un language pareil ta maman aurait mieux fait de t'avaler",
	"Avec un language pareil ta maman aurait mieux fait de t'avaler",
	"Chocolatine mecreant !!",
	"Pain au chocolat mecreant !!",
	"Qu'est ce que t'a pas compris ?",
	"Moi aussi je kiff leurs pates a la moisissure",
};

void	Server::Bot(const Client &sender, const std::string &msg)
{
	std::string		botname;
	size_t			i;

	i = 0;
	std::cout << "BOT" << std::endl;
	botname = "@Jesus@";
	while (i < 15)
	{
		if (msg.find(bot_find[i], 0) != std::string::npos)
			sendMessage(sender.getFd(), RPL_PRIVMSG(botname, sender.getNick(), bot_answer[i]));
		i++;
	}
}
