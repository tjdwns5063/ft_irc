#ifndef KILL_HPP
# define KILL_HPP

#include "ICommand.hpp"

class Kill: public ICommand {
public:
	Kill(Translator* translator, CommandType type);
	~Kill();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif