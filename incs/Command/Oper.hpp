#ifndef OPER_HPP
# define OPER_HPP

#include "ICommand.hpp"

class Oper: public ICommand {
public:
	Oper(Translator* translator, CommandType type);
	~Oper();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif