#ifndef JOIN_HPP
# define JOIN_HPP

#include "ICommand.hpp"

class Join: public ICommand {
public:
	Join(Translator* translator, CommandType type);
	~Join();
	int publishResultCode(std::vector<std::string>& cmd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif