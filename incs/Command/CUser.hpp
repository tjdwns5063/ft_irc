#ifndef CUSER_HPP
# define CUSER_HPP

#include "ICommand.hpp"

class CUser: public ICommand {
public:
	CUser(Translator* translator, CommandType type);
	~CUser();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif