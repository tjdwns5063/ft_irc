#include "Server.hpp"
#include "CUser.hpp"
#include "Command.hpp"

CUser::CUser(Translator* translator, CommandType type): ICommand(translator, type) {}

CUser::~CUser() {}

int CUser::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	std::string message;
    User* user = server.getUser(fd);

    if (cmd.size() < 5)
		return Translator::ERR_NEEDMOREPARAMS;
	else if (!user->getFlag(PASSED))
		return Translator::ERR_PASSWDMISMATCH;
    else if (user->getFlag(REGISTERD))
		return Translator::ERR_ALREADYREGISTERED;
	return Translator::DEFAULT;
}

void CUser::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd, fd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();

	if (code == Translator::DEFAULT) {
		message = translator->translateSuccess(nickName, cmd, *this);
		server.getUser(fd)->setUserName(cmd[1] + "@" + cmd[3]);
		server.getUser(fd)->setFlag(REGISTERD, true);
		send_fd(server, fd, message);
	}  else {
		if (code == Translator::ERR_PASSWDMISMATCH)
			server.getUser(fd)->setFlag(KILLED, true);
		message = translator->translateResult(nickName, code, cmd);
		send_fd(server, fd, message);
	}
}
