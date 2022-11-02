#include "Server.hpp"
#include "Pass.hpp"
#include "Command.hpp"

Pass::Pass(Translator* translator, CommandType type): ICommand(translator, type) {}

Pass::~Pass() {}

int Pass::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	if (cmd.size() < 2) {
		return Translator::ERR_NEEDMOREPARAMS;
    } else if (server.getUsers().find(fd)->second->getUserName() != "") {
		return Translator::ERR_ALREADYREGISTERED;
    } else if (server.getPassword() != cmd[1]) {
		return Translator::ERR_PASSWDMISMATCH;
    }
	return Translator::DEFAULT;
}

void Pass::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd, fd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();
    User* user = server.getUser(fd);

	if (code == Translator::DEFAULT) {
		message = translator->translateSuccess(nickName, cmd, *this);
        user->setFlag(PASSED, true);
    } else if (code == Translator::ERR_PASSWDMISMATCH) {
		message = translator->translateResult(nickName, code, cmd);
        user->setFlag(PASSED, false);
    } else {
		message = translator->translateResult(nickName, code, cmd);
	}
    send_fd(server, fd, message);
}
