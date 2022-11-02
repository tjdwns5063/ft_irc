#ifndef JOIN_HPP
# define JOIN_HPP

#include "ICommand.hpp"

class Join: public ICommand {
public:
	Join(Translator* translator, CommandType type);
	~Join();
	int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd);
	void execute(Server& server, std::vector<std::string>& cmd, int fd);
};

#endif