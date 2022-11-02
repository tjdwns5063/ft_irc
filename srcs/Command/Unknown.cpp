#include <map>
#include "Server.hpp"
#include "Unknown.hpp"
#include "Command.hpp"

Unknown::Unknown(Translator* translator, CommandType type): ICommand(translator, type) {}

Unknown::~Unknown() {}

void Unknown::execute(Server& server, std::vector<std::string>& cmd, int fd) {
    std::string message = translator->translateResult(server.getUser(fd)->getNickName(), Translator::ERR_UNKNOWNCOMMAND, cmd);
    send_fd(server, fd, message);
}
