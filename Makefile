# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agirona <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 18:15:54 by agirona           #+#    #+#              #
#    Updated: 2022/06/10 17:31:12 by agirona          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = Server

SRC = main.cpp Server.cpp Client.cpp Authentication.cpp Utility.cpp Exception.cpp \
	  Channel.cpp InitServer.cpp Routine.cpp CommandList.cpp Join.cpp PrivMsg.cpp \
	  Ping.cpp Nick.cpp User.cpp Part.cpp Mode.cpp Invite.cpp Notice.cpp Kick.cpp \
	  Topic.cpp Bot.cpp


PATH = .

HEADER = Server.hpp Client.hpp Channel.hpp

OBJ = $(SRC:.cpp=.o)

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address -I $(PATH)

%.o: %.cpp $(HEADER)
	c++ $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
			c++ $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re:		fclean all

.PHONY: all clean fclean re
