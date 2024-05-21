COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_BLUE=\033[0;34m
COLOUR_END=\033[0m

NAME			=	ircserv
CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -g
# -g -fsanitize=address 

PREFIX			=	srcs/
HPEFIX			=	include/

SOURCES_FILES	=	serv/main.cpp serv/Server.cpp serv/SocketEvents.cpp serv/Utils_Server.cpp clientito/client.cpp \
					channel/Channel.cpp channel/JoinChannel.cpp channel/ShowChannels.cpp \
					commands/command.cpp commands/kick.cpp commands/utils.cpp commands/topic.cpp
HEADERS_FILES	=	Server.hpp replies.hpp irc.hpp client.hpp Channel.hpp

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
	@echo "$(COLOUR_RED)==>Remove Obect Files...$(COLOUR_RED)$(COLOUR_END)"
	rm -f $(OBJECTS)
fclean: clean
	@echo "$(COLOUR_RED)==>Remove Executable...$(COLOUR_RED)$(COLOUR_END)"
	rm -f $(NAME)

re: fclean all