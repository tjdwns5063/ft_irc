#ifndef PART_HPP
# define PART_HPP

#include "ICommand.hpp"

class Part: public ICommand {
public:
	Part(Translator* translator, CommandType type);
	~Part();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif