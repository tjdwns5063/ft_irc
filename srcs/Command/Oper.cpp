#include <map>
#include "Server.hpp"
#include "Oper.hpp"
#include "Command.hpp"

Oper::Oper(Translator* translator, CommandType type): ICommand(translator, type) {}

Oper::~Oper() {}

int Oper::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	if (cmd.size() < 3) {
		return Translator::ERR_NEEDMOREPARAMS;
    }  else if (!server.searchUser(cmd[1])) {
		return Translator::ERR_NOSUCHNICK;
    } else if (server.getPassword() != cmd[2]) {
		return Translator::ERR_PASSWDMISMATCH;
    } else if (cmd[1] != server.getUser(fd)->getNickName() && !server.getUser(fd)->getFlag(OP)) {
		return Translator::ERR_NOPRIVILEGES;
	}
	return Translator::DEFAULT;
}

void Oper::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd, fd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();
	int targetFd;

	if (code == Translator::DEFAULT) {
		message = translator->translateSuccess(nickName, cmd, *this);
	    server.getUser(cmd[1])->setFlag(OP, true);
        targetFd = server.getUser(cmd[1])->getFd();
	}  else {
		message = translator->translateResult(nickName, code, cmd);
        targetFd = fd;
	}
	send_fd(server, targetFd, message);
}
