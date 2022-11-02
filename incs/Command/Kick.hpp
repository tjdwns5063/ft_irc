#ifndef KICK_HPP
# define KICK_HPP

#include "ICommand.hpp"

class Kick: public ICommand {
public:
	Kick(Translator* translator, CommandType type);
	~Kick();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif