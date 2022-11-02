#ifndef QUIT_HPP
# define QUIT_HPP

#include "ICommand.hpp"

class Quit: public ICommand {
public:
	Quit(Translator* translator, CommandType type);
	~Quit();
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif