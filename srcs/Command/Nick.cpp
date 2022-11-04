#include <map>
#include "Server.hpp"
#include "Nick.hpp"
#include "Command.hpp"

Nick::Nick(Translator* translator, CommandType type): ICommand(translator, type) {}

Nick::~Nick() {}

int Nick::publishResultCode(Server& server, std::vector<std::string>& cmd) {
    if (cmd.size() < 2)
		return Translator::ERR_NEEDMOREPARAMS;
	if (server.searchUser(cmd[1]))
		return Translator::ERR_NICKNAMEINUSE;
	return Translator::DEFAULT;
}

void Nick::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();

	if (code == Translator::DEFAULT) {
		message = translator->translateSuccess(nickName, cmd, *this);
		server.getUser(fd)->setNickName(cmd[1]);
    	send_all(server, message);
	}  else {
		message = translator->translateResult(nickName, code, cmd);
        send_fd(server, fd, message);
	}
}
