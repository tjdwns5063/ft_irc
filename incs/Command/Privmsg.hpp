#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

#include "ICommand.hpp"

class Privmsg: public ICommand {
public:
	Privmsg(Translator* translator, CommandType type);
	~Privmsg();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif