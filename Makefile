COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_BLUE=\033[0;34m
COLOUR_END=\033[0m

NAME			=	server
CFLAGS			=	-Wall -Wextra -Werror -std=c++98

PREFIX			=	srcs/
HPEFIX			=	include/

SOURCES_FILES	=	serv/Server.cpp serv/main.cpp commands/main.cpp commands/server.cpp commands/utils.cpp
HEADERS_FILES	=	Server.hpp header.hpp irc.hpp

SOURCES			=	$(addprefix $(PREFIX), $(SOURCES_FILES))
HEADERS			=	$(addprefix $(HPEFIX), $(HEADERS_FILES))
OBJECTS			= 	$(SOURCES:.cpp=.o)


all: $(NAME) $(HEADERS)

$(NAME): $(OBJECTS) $(HEADERS)
	@echo "$(COLOUR_GREEN)==>Linking...$(COLOUR_RED)$(COLOUR_END)"
	c++ $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo "$(COLOUR_GREEN)==>Compilation Complete: run $(COLOUR_RED)./$(NAME)$(COLOUR_END)"

%.o:%.cpp $(HEADERS)
	@echo "$(COLOUR_GREEN)==>Compiling... $(COLOUR_END)"
	c++ $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
fclean: clean
	rm -f $(NAME)

re: fclean all