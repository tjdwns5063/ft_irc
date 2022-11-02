#ifndef UNKNOWN_HPP
# define UNKNOWN_HPP

#include "ICommand.hpp"

class Unknown: public ICommand {
public:
	Unknown(Translator* translator, CommandType type);
	~Unknown();
	int publishResultCode(Server& server, std::vector<std::string>& cmd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif