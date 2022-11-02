#include <map>
#include "Server.hpp"
#include "Join.hpp"
#include "Command.hpp"

Join::Join(Translator* translator, CommandType type): ICommand(translator, type) {}

Join::~Join() {}

int Join::publishResultCode(std::vector<std::string>& cmd) {
    if (cmd.size() < 2)
		return Translator::ERR_NEEDMOREPARAMS;
    else if (cmd[1][0] != '#')
        return Translator::ERR_NOSUCHCHANNEL;
    return Translator::DEFAULT;
}

void Join::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(cmd);
	std::string message;
	const std::string& nickName = server.getUser(fd)->getNickName();

	if (code == Translator::DEFAULT) {
		Channel* channel = server.getChannel(cmd[1]);

		message = translator->translateSuccess(nickName, cmd, *this);
		channel->addUser(*server.getUser(fd));
        server.getUser(fd)->addChannel(*channel);
        send_channel(server, *channel, message);
	}  else {
		message = translator->translateResult(nickName, code, cmd);
        send_fd(server, fd, message);
	}
}
