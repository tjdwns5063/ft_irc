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

void send_fd(Server &server, int fd, string s)
{
    server.getUser(fd).setBuf(s);
    server.addEvents(fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
}

void cmd_nick(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    User &user = server.getUser(fd);
    if (cmd.size() < 2)
    {
        s = translateResult(user.getNickName(), ERR_NEEDMOREPARAMS, cmd);
        send_fd(server, fd, s);
        return ;       
    }
    if (user.getNickName() == cmd[1])
        return ;
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s + "\n";
	user.setNickName(cmd[1]);
    send_allChannel(server, user, s);
}

void cmd_user(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    User &user = server.getUser(fd);
    if (cmd.size() < 5)
    {
        s = translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
        send_fd(server, fd, s);
        return ;
    }
    if (!user.getUserName().empty())
        return ;
    map<int, User>::iterator it;
    for (it = server.getUsers().begin(); it != server.getUsers().end(); it++)
    {
        if (it->second.getNickName() == cmd[1])
        {
            s = translateResult(user.getNickName(), ERR_NOSUCHCHANNEL, cmd);
            send_fd(server, fd, s);
            return ;
        }
    }
    user.setUserName(cmd[1] + "@" + cmd[3]);
    s = ":" + user.getNickName() + " 001 " + user.getNickName() + "\n";
    user.setBuf(s);
    server.addEvents(fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
}

void cmd_join(Server &server, int fd, std::vector<std::string> cmd)
{
    User &user = server.getUser(fd);
    string s;
    if (cmd.size() < 2)
    {
        s = translateResult(user.getNickName(), ERR_NEEDMOREPARAMS, cmd);
        send_fd(server, fd, s);
        return ;
    }
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        channel.addUser(server.getUser(fd));
        server.getUser(fd).addChannel(channel);
        std::string message = ":" + server.getUser(fd).getNickName() + " JOIN " + " :" + cmd[1] + "\n";
        send_channel(server, channel, message);
    }
    else {
        // error 403
        s = translateResult(user.getNickName(), ERR_NOSUCHCHANNEL, cmd);
        send_fd(server, fd, s);
        return ;
    }
}

void cmd_privmsg(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    User &user = server.getUser(fd);
    if (cmd.size() < 3)
    {
        s = translateResult(user.getNickName(), ERR_NEEDMOREPARAMS, cmd);
        send_fd(server, fd, s);
        return ;
    }
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s; 
    s += "\n";
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        if (channel.chkUser(fd) == false)
        {
            //error 404
            s = translateResult(user.getNickName(), ERR_CANNOTSENDTOCHAN, cmd);
            send_fd(server, fd, s);
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
            s = translateResult(server.getUser(fd).getNickName(), ERR_NOSUCHNICK, cmd);
            send_fd(server, fd, s);
            return ; 
        }
        User &rcvuser = server.getUser(rcvfd);
        rcvuser.setBuf(s);
        server.addEvents(rcvfd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
}

void cmd_part(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    std::string msg; 
    User& user = server.getUser(fd);
    Channel &channel = server.getChannel(cmd[1]);

    if (cmd.size() < 2) {
        msg = translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
        send_fd(server, fd, msg);
        return ;
    }
    msg = ":" + user.getNickName() + " PART " + cmd[1] + "\n";
    send_channel(server, channel, msg);
    user.leaveChannel(channel);
    channel.removeUser(user);
}

void cmd_quit(Server &server, int fd)
{
    User &user = server.getUser(fd);
    map<string, Channel> &Channels = server.getChannels();
    std::string message = ":" + user.getNickName() + " QUIT :Quit Bye Bye\n";
    // send(fd, message.c_str(), message.length(), 0);
    for (map<string, Channel>::iterator it = Channels.begin(); it != Channels.end(); it++)
    {
        it->second.removeUser(user);
    }
    server.removeUser(fd);
    close(fd);
    send_all(server, message);
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
    message = ":" + user.getNickName() + " KICK " + cmd[1] + " " + cmd[2];
    if (cmd.size() > 3)
        message += (" :" + cmd[2] + "\n");
    else
        message += "\n";

    return std::make_pair(message, DEFAULT);
}

void cmd_kick(Server &server, int fd, std::vector<std::string>& cmd)
{
    Channel &channel = server.getChannel(cmd[1]);
    pair<std::string, ResultCode> message = makeKickMessage(server, cmd, fd);
    if (message.second == DEFAULT) {
        send_channel(server, channel, message.first.c_str());
        channel.removeUser(cmd[2]);
        channel.searchUser(cmd[2])->leaveChannel(channel);
    } else {
        send_fd(server, fd, message.first);
    }
}

std::pair<std::string, ResultCode> makePassMesaage(Server &server, vector<std::string>& cmd, int& fd) {
    std::string message;
    const std::string& pw = server.getPassword();
    User &user = server.getUser(fd);
    if (cmd.size() < 2) {
        message = ":localhost 461 " + user.getNickName() + " :Not enough parameters\n";
        return make_pair(message, ERR_NEEDMOREPARAMS);
    } else if (pw != cmd[1]) {
        message = ":localhost 464 " + user.getNickName() + " :Password incorrect\n";
        return make_pair(message, ERR_PASSWDMISMATCH);
    }
    if (server.getUsers().find(fd)->second.getUserName() != "") {
        message = ":localhost 462 " + user.getNickName() + " :You may not reregister\n";
        return make_pair(message, ERR_ALREADYREGISTERED);
    }
    message = ":localhost :Password is Correct\n";
    return (make_pair(message, DEFAULT));
}

std::pair<std::string, ResultCode> makeOperMessage(Server& server, vector<string> cmd) {
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
    send_fd(server, fd, message.first);
}

void cmd_oper(Server &server, int fd, std::vector<std::string>& cmd) {
    std::pair<std::string, ResultCode> message;
    int targetFd;
    
    message = makeOperMessage(server, cmd);
    if (message.second == DEFAULT)
        targetFd = server.getUser(cmd[1]).getFd();
    else
        targetFd = fd;
    send_fd(server, targetFd, message.first);
}

std::pair<std::string, ResultCode> makeKillMessage(Server& server, vector<string> cmd, int fd) {
    std::string message;
    User& user = server.getUser(fd);

    if (cmd.size() < 2) {
        return make_pair(translateResult(user.getNickName(), ERR_NEEDMOREPARAMS, cmd), ERR_NEEDMOREPARAMS);
    } else if (cmd[1] != server.getUser(cmd[1]).getNickName()) {
        return make_pair(translateResult(user.getNickName(), ERR_NOSUCHNICK, cmd), ERR_NOSUCHNICK);
    } else if (!user.getOp()) {
        return make_pair(translateResult(user.getNickName(), ERR_NOPRIVILEGES, cmd), ERR_NOPRIVILEGES);
    }
    // user.setOp(true);
    message = ":" + user.getNickName() + " KILL " + cmd[1] + " :" + user.getNickName() + "\n";
    return make_pair(message, DEFAULT);
}

void cmd_kill(Server &server, int fd, std::vector<std::string>& cmd) {
    std::pair<std::string, ResultCode> message = makeKillMessage(server, cmd, fd);
    int targetFd;

    std::cout << message.first << '\n';
    if (message.second == DEFAULT) {
        targetFd = server.getUser(cmd[1]).getFd();
        std::string quitMsg = ":" + server.getUser(targetFd).getNickName() + " QUIT :Killed " + server.getUser(fd).getNickName() + "\n";
        std::cout << "quitMsg: " << quitMsg;
        send_fd(server, targetFd, message.first);
        map<int, User>::iterator it = server.getUsers().begin();
        for (; it != server.getUsers().end(); ++it) {
            if (it->first == targetFd)
                continue ;
            send_fd(server, it->first, quitMsg);
        }
        // send_all(server, quitMsg);
        server.getUser(targetFd).setKilled(true);
        // send(targetFd, message.first.c_str(), message.first.length(), 0);
    }  else {
        send_fd(server, targetFd, message.first);
        // send(fd, message.first.c_str(), message.first.length(),  0);
    }
}

void cmd_unknown(Server& server, int fd, std::vector<std::string>& cmd) {
    std::string message = translateResult(server.getUser(fd).getNickName(), ERR_UNKNOWNCOMMAND, cmd);
    send_fd(server, fd, message);
}
