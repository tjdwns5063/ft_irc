#include "Server.hpp"
#include "Privmsg.hpp"
#include "Command.hpp"

Privmsg::Privmsg(Translator* translator, CommandType type): ICommand(translator, type) {}

Privmsg::~Privmsg() {}

int Privmsg::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	std::string message;
    
    if (cmd.size() < 3)
		return Translator::ERR_NEEDMOREPARAMS;
	else if (cmd[1].c_str()[0] == '#') {
		Channel *channel = server.getChannel(cmd[1]);
        if (channel->chkUser(fd) == false) {
			return Translator::ERR_CANNOTSENDTOCHAN;
		}
	}
	else if (!server.searchUser(cmd[1]))
		return Translator::ERR_NOSUCHNICK;
	return Translator::DEFAULT;
}

void Privmsg::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd, fd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();

	if (code == Translator::DEFAULT) {
		message = translator->translateSuccess(nickName, cmd, *this);
		if (cmd[1][0] == '#') {
			send_excludeme(server, *server.getChannel(cmd[1]), *server.getUser(fd), message);
		} else {
			int targetFd = server.getUser(cmd[1])->getFd();
			send_fd(server, targetFd, message);
		}
	}  else {
		message = translator->translateResult(nickName, code, cmd);
        send_fd(server, fd, message);
	}
}
