#include <map>
#include "Server.hpp"
#include "Part.hpp"
#include "Command.hpp"

Part::Part(Translator* translator, CommandType type): ICommand(translator, type) {}

Part::~Part() {}

int Part::publishResultCode(std::vector<std::string>& cmd) {
	std::string message;
    
    if (cmd.size() < 2)
		return Translator::ERR_NEEDMOREPARAMS;
	return Translator::DEFAULT;
}

void Part::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(cmd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();

	if (code == Translator::DEFAULT) {
	    Channel *channel = server.getChannel(cmd[1]);

		message = translator->translateSuccess(nickName, cmd, *this);
		send_channel(server, *channel, message);
		server.getUser(fd)->leaveChannel(*channel);
		channel->removeUser(*server.getUser(fd));
	}  else {
		message = translator->translateResult(nickName, code, cmd);
        send_fd(server, fd, message);
	}
}
