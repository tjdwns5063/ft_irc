#ifndef QUIT_HPP
# define QUIT_HPP

#include "ICommand.hpp"

class Quit: public ICommand {
public:
	Quit(Translator* translator, CommandType type);
	~Quit();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif