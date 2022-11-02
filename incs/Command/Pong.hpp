#ifndef PONG_HPP
# define PONG_HPP

#include "ICommand.hpp"

class Pong: public ICommand {
public:
	Pong(Translator* translator, CommandType type);
	~Pong();
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif