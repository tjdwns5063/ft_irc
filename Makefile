SERVER=server
FLAG=-Wall -Wextra -Werror
INCLUDE=-I./incs/ -I./incs/Command/

COMMAND_SRCS_NAME = CUser.cpp Join.cpp Kick.cpp Kill.cpp Nick.cpp Oper.cpp Part.cpp Pass.cpp Pong.cpp Privmsg.cpp Quit.cpp ICommand.cpp Unknown.cpp

COMMAND_SRCS=$(addprefix ./srcs/Command/, $(COMMAND_SRCS_NAME))
SERVER_SRCS=$(addprefix ./srcs/, Server.cpp User.cpp Channel.cpp main.cpp Translator.cpp utils.cpp Command.cpp)
SRCS = $(COMMAND_SRCS) $(SERVER_SRCS)
OBJS=$(SRCS:.cpp=.o)

all: $(SERVER)

$(SERVER): $(OBJS)
	c++ $(FLAG) $(INCLUDE) $(OBJS) -o $(SERVER)

%.o: %.cpp
	c++ -c $(FLAG) $(INCLUDE) $< -o $@

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS)
	rm -rf $(SERVER)

re: fclean all