#include <map>
#include "Server.hpp"
#include "Kick.hpp"
#include "Command.hpp"

Kick::Kick(Translator* translator, CommandType type): ICommand(translator, type) {}

Kick::~Kick() {}

int Kick::publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) {
	  std::string message;
    map<string, Channel*>& channels = server.getChannels();
    User* user = server.getUser(fd);
    std::string nickname = user->getNickName();
    map<string, Channel*>::iterator targetChannelIter = channels.find(cmd[1]);

    if (cmd.size() < 3) {
		return Translator::ERR_NEEDMOREPARAMS;
    } else if (targetChannelIter == channels.end()) {
		return Translator::ERR_NOSUCHCHANNEL;
    } else if (!user->getFlag(OP)) {
		return Translator::ERR_CHANOPRIVSNEEDED;
    } else {
        Channel* targetChannel = targetChannelIter->second;
        // 내가 그 채널에 있는지
        std::cout << "nickname " << user->getNickName() << '\n';
        if (targetChannel->searchUser(user->getNickName()) == targetChannel->getUsers().end()) {
            return translator->ERR_NOTONCHANNEL;
        }
        // target이 그 채널에 있는지
        if (targetChannel->searchUser(cmd[2]) == targetChannel->getUsers().end()) {
			return Translator::ERR_USERNOTINCHANNEL;
        }
    }
	  return Translator::DEFAULT;
}

void Kick::execute(Server& server, std::vector<std::string>& cmd, int fd) {
    int code = publishResultCode(server, cmd, fd);
    std::string message;
    const std::string& nickName = server.getUser(fd)->getNickName();
    Channel* channel = server.getChannel(cmd[1]);

    if (code == Translator::DEFAULT) {
        message = translator->translateSuccess(nickName, cmd, *this);
        send_channel(server, *channel, message);
        (*channel->searchUser(cmd[2]))->leaveChannel(*channel);
        channel->removeUser(cmd[2]);
    } else {
        message = translator->translateResult(nickName, code, cmd);
        send_fd(server, fd, message);
    }
}
