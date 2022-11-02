#ifndef PASS_HPP
# define PASS_HPP

#include "ICommand.hpp"

class Pass: public ICommand {
public:
	Pass(Translator* translator, CommandType type);
	~Pass();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif