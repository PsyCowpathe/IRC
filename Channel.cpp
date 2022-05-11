/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:58:20 by agirona           #+#    #+#             */
/*   Updated: 2022/05/11 20:30:32 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
	std::cout << "New channel created !" << std::endl;
}

Channel::~Channel()
{
	std::cout << "Channel destructed !" << std::endl;
}
