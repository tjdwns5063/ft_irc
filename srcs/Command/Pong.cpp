#include "Server.hpp"
#include "Pong.hpp"
#include "Command.hpp"

Pong::Pong(Translator* translator, CommandType type): ICommand(translator, type) {}

Pong::~Pong() {}

int Pong::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	return Translator::DEFAULT;
}

void Pong::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd, fd);
	std::string message;
	const std::string& nickName = server.getUser(fd).getNickName();

	message = translator->translateSuccess(nickName, cmd, *this);
	send_fd(server, fd, message);
}
