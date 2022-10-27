SERVER=server
CLIENT=client
FLAG=-Wall -Wextra #-Werror
INCLUDE=-I./incs/

SERVER_SRCS=$(addprefix ./srcs/, Server.cpp User.cpp Channel.cpp main.cpp Translator.cpp utils.cpp Command.cpp)
CLIENT_SRCS=$(addprefix ./srcs/, practice_client.cpp)

SERVER_OBJS=$(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS=$(CLIENT_SRCS:.cpp=.o)

$(SERVER): $(SERVER_OBJS)
	c++ $(FLAG) $(INCLUDE) $(SERVER_OBJS) -o $(SERVER)

$(CLIENT): $(CLIENT_OBJS)
	c++ $(FLAG) $(INCLUDE) $(CLIENT_OBJS) -o $(CLIENT)

%.o: %.cpp
	c++ -c $(FLAG) $(INCLUDE) $< -o $@

clean:
	rm -rf $(SERVER_OBJS) $(CLIENT_OBJS)

fclean:
	rm -rf $(SERVER_OBJS) $(CLIENT_OBJS)
	rm -rf $(SERVER) $(CLIENT)
