#include <map>
#include "Server.hpp"
#include "Kill.hpp"
#include "Command.hpp"

Kill::Kill(Translator* translator, CommandType type): ICommand(translator, type) {}

Kill::~Kill() {}

int Kill::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	std::string message;
    
    if (cmd.size() < 2) {
		return Translator::ERR_NEEDMOREPARAMS;
    } else if (cmd[1] != server.getUser(cmd[1])->getNickName()) {
		return Translator::ERR_NOSUCHNICK;
    } else if (!server.getUser(fd)->getFlag(OP)) {
		return Translator::ERR_NOPRIVILEGES;
    }
	return Translator::DEFAULT;
}

void Kill::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd, fd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();
        

	if (code == Translator::DEFAULT) {
		int targetFd = server.getUser(cmd[1])->getFd();
		message = translator->translateSuccess(nickName, cmd, *this);
		std::string quitMsg = ":" + server.getUser(targetFd)->getNickName() + " QUIT :Killed " + server.getUser(fd)->getNickName() + "\n";
        std::cout << "quitMsg: " << quitMsg;
        
		send_fd(server, server.getUser(cmd[1])->getFd(), message);
        map<int, User*>::iterator it = server.getUsers().begin();
        for (; it != server.getUsers().end(); ++it) {
            if (it->first == targetFd)
                continue ;
            send_fd(server, it->first, quitMsg);
        }
        server.getUser(targetFd)->setFlag(KILLED, true);
	}  else {
		message = translator->translateResult(nickName, code, cmd);
        send_fd(server, fd, message);
	}
}
