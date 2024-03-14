# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/13 21:22:05 by abelfany          #+#    #+#              #
#    Updated: 2024/03/14 22:04:57 by abelfany         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
HEADER = ./include/header.hpp ./include/irc.hpp
RM = @rm -rf

SRCS = ./srcs/main.cpp ./srcs/server.cpp ./srcs/utils.cpp

FLAG = -Wall -Wextra -Werror -std=c++98

S_OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

%.o : %.cpp $(HEADER)
	c++ $(FLAG) -c $< -o $@

$(NAME):$(S_OBJ)
	c++ $(FLAG) $(S_OBJ) -o $(NAME)

clean:
	$(RM) $(S_OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all fclean clean re
