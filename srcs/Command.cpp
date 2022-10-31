#include "Command.hpp"

void send_all(Server &server, std::string s)
{
	std::map<int, User> &users = server.getUsers();

	for (std::map<int, User>::iterator it = users.begin(); it != users.end(); it++)
	{
        server.getUser(it->second.getFd()).setBuf(s);
        server.addEvents(it->second.getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	}
}

void send_allChannel(Server &server, User &user, string s)
{
    std::vector<Channel> &channels = user.getChannels();
    std::cout << "channel size: " << channels.size() << std::endl;
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::vector<User> &users = it->getUsers();
        for (std::vector<User>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            server.getUser(it2->getFd()).setBuf(s);
            server.addEvents(it2->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
            // write(it2->getFd(), s.c_str(), s.length());
        }
    }
}

void send_channel(Server &server, Channel &channel, string s)
{
    for (std::vector<User>::iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++)
    {
        server.getUser(it->getFd()).setBuf(s);
        server.addEvents(it->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
}

void send_excludeme(Server &server, Channel &channel, User &user, string s)
{
    for (std::vector<User>::iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++)
    {
        if (it->getFd() == user.getFd())
            continue ;
        server.getUser(it->getFd()).setBuf(s);
        server.addEvents(it->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
}

void cmd_nick(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 2)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    User &user = server.getUser(fd);
    if (user.getNickName() == cmd[1])
        return ;
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s + "\n";
	user.setNickName(cmd[1]);
    std::cout << "nick response: " << s << std::endl;
    send_allChannel(server, user, s);
    std::cout << "nickname : " << user.getNickName() << std::endl;
}

void cmd_user(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 5)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    if (!server.getUser(fd).getUserName().empty())
        return ;
    server.getUser(fd).setUserName(cmd[1] + "@" + cmd[3]);
    std::cout << "username : " << server.getUser(fd).getUserName() << std::endl;
    s = ":" + server.getUser(fd).getNickName() + " 001 " + server.getUser(fd).getNickName() + "\n";
    // s = ":001 " + server.getUser(fd).getNickName() + "\n";
    server.getUser(fd).setBuf(s);


    server.addEvents(fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    // write(fd, s.c_str(), s.length());
}

void cmd_join(Server &server, int fd, std::vector<std::string> cmd)
{
    if (cmd.size() < 2)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        channel.addUser(server.getUser(fd));
        server.getUser(fd).addChannel(channel);
    }
    else {
        // error 403
        translateResult(server.getUser(fd).getNickName(), ERR_NOSUCHCHANNEL, cmd);
    }
}

void cmd_privmsg(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 3)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    User &user = server.getUser(fd);
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s; 
    s += "\n";
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        std::vector<User> &users = channel.getUsers();
        std::cout << "user size: " << users.size() << std::endl;
        if (channel.chkUser(fd) == false)
        {
            //error 404
            translateResult(user.getNickName(), ERR_CANNOTSENDTOCHAN, cmd);
            return ;
        }
        send_excludeme(server, channel, user, s);
    }
    else
    {
        int rcvfd = -1;
        map<int, User>::iterator it;
        for (it = server.getUsers().begin(); it != server.getUsers().end(); ++it) {
            if (it->second.getNickName() == cmd[1])
            {
                rcvfd = it->second.getFd();
                break ;
            }
        }
        if (rcvfd == -1)
        {
            translateResult(server.getUser(fd).getNickName(), ERR_NOSUCHNICK, cmd);
            return ; 
        }
        User &rcvuser = server.getUser(rcvfd);
        rcvuser.setBuf(s);
        server.addEvents(rcvfd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
}

void cmd_part(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 2)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    Channel &channel = server.getChannel(cmd[1]);
    User &user = server.getUser(fd);
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s + "\n"; 
    std::cout << s << std::endl;
    send_channel(server, channel, s);
    user.leaveChannel(channel);
    channel.removeUser(user);
}

void cmd_quit(Server &server, int fd)
{
    User &user = server.getUser(fd);
    map<string, Channel> &Channels = server.getChannels();
    for (map<string, Channel>::iterator it = Channels.begin(); it != Channels.end(); it++)
    {
        it->second.removeUser(user);
    }
    server.removeUser(fd);
    close(fd);
}

std::pair<std::string, ResultCode> makeKickMessage(Server& server, vector<string> cmd, int fd) {
    std::string message;
    map<string, Channel>& channels = server.getChannels();
    User& user = server.getUser(fd);
    std::string nickname = user.getNickName();
    map<string, Channel>::iterator targetChannelIter = channels.find(cmd[1]);

    if (cmd.size() < 3) {
        return std::make_pair(translateResult(nickname, ERR_NEEDMOREPARAMS, cmd), ERR_NEEDMOREPARAMS);
    } else if (targetChannelIter == channels.end()) {
        return std::make_pair(translateResult(nickname, ERR_NOSUCHCHANNEL, cmd), ERR_NOSUCHCHANNEL);
    } else if (!user.getOp()) {
        return std::make_pair(translateResult(nickname, ERR_CHANOPRIVSNEEDED, cmd), ERR_CHANOPRIVSNEEDED);
    } else {
        Channel& targetChannel = targetChannelIter->second;
        // 내가 그 채널에 있는지
        if (targetChannel.searchUser(user.getNickName()) == targetChannel.getUsers().end()) {
            return std::make_pair(translateResult(nickname, ERR_NOTONCHANNEL, cmd), ERR_NOTONCHANNEL);
        }
        // target이 그 채널에 있는지
        if (targetChannel.searchUser(cmd[2]) == targetChannel.getUsers().end()) {
            return std::make_pair(translateResult(nickname, ERR_USERNOTINCHANNEL, cmd), ERR_USERNOTINCHANNEL);
        }
    }
    message = ":" + server.getUser(fd).getNickName() + " KICK " + cmd[1] + " " + cmd[2];
    if (cmd.size() > 3)
        message += (" :" + cmd[2] + "\n");
    else
        message += "\n";
    std::cout << "kick message: " << message;

    return std::make_pair(message, DEFAULT);
}

void cmd_kick(Server &server, int fd, std::vector<std::string>& cmd)
{
    pair<std::string, ResultCode> message = makeKickMessage(server, cmd, fd);
    if (message.second == DEFAULT) {
        send_channel(server, server.getChannel(cmd[1]), message.first.c_str());
        server.getChannel(cmd[1]).removeUser(cmd[2]);
        server.getChannel(cmd[1]).searchUser(cmd[2])->leaveChannel(server.getChannel(cmd[1]));
    } else {
        send(fd, message.first.c_str(), message.first.length(), 0);
    }
}

std::pair<std::string, ResultCode> makePassMesaage(Server &server, vector<std::string>& cmd, int& fd) {
    std::string message;
    const std::string& pw = server.getPassword();

    if (cmd.size() < 2) {
        message = ":localhost 461 " + server.getUser(fd).getNickName() + " :Not enough parameters\n";
        return make_pair(message, ERR_NEEDMOREPARAMS);
    } else if (pw != cmd[1]) {
        message = ":localhost 464 " + server.getUser(fd).getNickName() + " :Password incorrect\n";
        return make_pair(message, ERR_PASSWDMISMATCH);
    }
    if (server.getUsers().find(fd)->second.getUserName() != "") {
        message = ":localhost 462 " + server.getUser(fd).getNickName() + " :You may not reregister\n";
        return make_pair(message, ERR_ALREADYREGISTERED);
    }
    message = ":localhost :Password is Correct\n";
    return (make_pair(message, DEFAULT));
}

std::pair<std::string, ResultCode> makeOperMessage(Server& server, vector<string> cmd, int fd) {
    std::string message;
    const std::string& pw = server.getPassword();
    User& user = server.getUser(cmd[1]);

    if (cmd.size() < 3) {
        return make_pair(translateResult(user.getNickName(), ERR_NEEDMOREPARAMS, cmd), ERR_NEEDMOREPARAMS);
    } else if (cmd[1] != user.getNickName()) {
        return make_pair(translateResult(user.getNickName(), ERR_NOSUCHNICK, cmd), ERR_NOSUCHNICK);
    } else if (pw != cmd[2]) {
        return make_pair(translateResult(user.getNickName(), ERR_PASSWDMISMATCH, cmd), ERR_PASSWDMISMATCH);
    } 
    user.setOp(true);
    message = ":localhost 381 " + cmd[1] + " :You are now an IRC operator\n";
    return make_pair(message, DEFAULT);
}

void cmd_pass(Server &server, int fd, std::vector<std::string>& cmd) {
    std::pair<std::string, ResultCode> message;

    message = makePassMesaage(server, cmd, fd);
    send(fd, message.first.c_str(), message.first.length(), 0);
}

void cmd_oper(Server &server, int fd, std::vector<std::string>& cmd) {
    std::pair<std::string, ResultCode> message;
    int targetFd;
    
    message = makeOperMessage(server, cmd, fd);
    if (message.second == DEFAULT)
        targetFd = server.getUser(cmd[1]).getFd();
    else
        targetFd = fd;
    // std::cout << "targetFd: " << targetFd << '\n';
    // std::cout << "string: " << message.first;
    send(targetFd, message.first.c_str(), message.first.length(), 0);
}
