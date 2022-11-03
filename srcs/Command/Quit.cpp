#include <map>
#include "Server.hpp"
#include "Quit.hpp"
#include "Command.hpp"

Quit::Quit(Translator* translator, CommandType type): ICommand(translator, type) {}

Quit::~Quit() {}

void Quit::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();
	User *user = server.getUser(fd);

    message = translator->translateSuccess(nickName, cmd, *this);
    user->setFlag(KILLED, true);
    send_all(server, message);
}
