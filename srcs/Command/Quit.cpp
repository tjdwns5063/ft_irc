#include <map>
#include "Server.hpp"
#include "Quit.hpp"
#include "Command.hpp"

Quit::Quit(Translator* translator, CommandType type): ICommand(translator, type) {}

Quit::~Quit() {}

int Quit::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	return Translator::DEFAULT;
}

void Quit::execute(Server& server, std::vector<std::string>& cmd, int fd) {
	int code = publishResultCode(server, cmd, fd);
	std::string message;
	const std::string& nickName = server.getUser(fd).getNickName();
	User &user = server.getUser(fd);
    map<string, Channel> &Channels = server.getChannels();

    message = translator->translateSuccess(nickName, cmd, *this);
    for (map<string, Channel>::iterator it = Channels.begin(); it != Channels.end(); it++)
    {
        it->second.removeUser(user);
    }
    server.removeUser(fd);
    close(fd);
    send_all(server, message);
}
