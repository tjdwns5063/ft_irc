#ifndef NICK_HPP
# define NICK_HPP

#include "ICommand.hpp"

class Nick: public ICommand {
public:
	Nick(Translator* translator, CommandType type);
	~Nick();
	int publishResultCode(Server& server, std::vector<std::string>& cmd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif