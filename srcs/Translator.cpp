#include "Translator.hpp"

std::string translateResult(std::string& nickName, int result, std::string& cmd) {
    
}

std::string makePassMesaage(Server &server, vector<std::string>& cmd, int& fd) {
    std::string message;
    const std::string& pw = server.getPassword();

    if (cmd.size() < 2) {
        message = ":localhost 461 " + server.getUser(fd).getNickName() + " :Not enough parameters\n";
    } else if (pw != cmd[1]) {
        message = ":localhost 464 " + server.getUser(fd).getNickName() + " :Password incorrect\n";
    }
    if (server.getUsers2().find(fd)->second.getUserName() != "") {
        message += ":localhost 462 " + server.getUser(fd).getNickName() + " :You may not reregister\n";
    }
    return (message);
}

std::string makeOperMessage(Server& server, vector<string> cmd, int fd) {
    std::string message;
    const std::string& pw = server.getPassword();
    User& user = server.getUser(fd);

    if (cmd.size() < 3) {
        message = ":localhost 461 " + user.getNickName() + " :Not enough parameters\n";
    } else if (pw != cmd[2]) {
        message = ":localhost 464 " + user.getNickName() + " :Password incorrect\n";
    } else {
        message = ":localhost 381 " + user.getNickName() + " :You are now an IRC operator\n";
        user.setOp(true);
    }
    return (message);
}

int makeKickMessage(Server& server, vector<string> cmd, int fd) {
    std::string message;
    map<string, Channel>& channels = server.getChannels();
    User& user = server.getUser(fd);
    map<string, Channel>::iterator targetChannelIter = channels.find(cmd[1]);

    if (cmd.size() < 3) {
        return 461;
        // message = ":localhost 461 " + user.getNickName() + " :Not enough parameters\n";
    } else if (targetChannelIter == channels.end()) {
        return 403;
        // message = ":localhost 403 " + user.getNickName() + " :No such channel\n";
    } else if (!user.getOp()) {
        return 482;
        // message = ":localhost 482 " + user.getNickName() + " :You're not Operator\n";
    } else {
        Channel& targetChannel = targetChannelIter->second;

        // 내가 그 채널에 있는지
        vector<User>::const_iterator found = find(targetChannel.getUsers().begin(), targetChannel.getUsers().end(), user);
        if (found == targetChannel.getUsers().end()) {
            return 442;
            // message = ":localhost 442 " + user.getNickName() + " :You're not on that channel\n";
            // return (message);
        }
        // target이 그 채널에 있는지
                // :seongjki!seongjki@121.135.181.35 KICK #a seongjki_ :seongjki
        if (targetChannel.searchUser(cmd[2]) == targetChannel.getUsers().end()) {
            return 441;
            // message = ":" + user.getUserName() + " KICK " + cmd[1] + " " + cmd[2];
            // if (cmd.size() > 3) {
            //     message += " :" + cmd[3] + "\n";
            // } else {
            //     message += " :" + user.getUserName() + "\n";
            // }
            // return (message);
        }
        // message = ":localhost 441 " + user.getNickName() + " :They aren't on that channel\n";
    }
    return 0;
}

int request(Server &server, int fd, vector<std::string> cmd)
{
    int n = 0;

    for (int i = 0 ; i < cmd.size(); i++)
    {
        std::cout << "cmd" << i << ": " << cmd[i] << std::endl;
    }
    if (cmd[0] == "PASS")
    {
        std::string message = makePassMesaage(server, cmd, fd);
        write(fd, message.c_str(), message.length());
    }
    else if (cmd[0] == "OPER")
    {
        std::string message = makeOperMessage(server, cmd, fd);
        write(fd, message.c_str(), message.length());
    }
    else if (cmd[0] == "KICK")
    {
        int result = makeKickMessage(server, cmd, fd);
        std::string translateResult(result, cmd[0]);
        std::cout << "size: " << server.getChannel(cmd[1]).getUsers().size() << '\n';
        for (vector<User>::const_iterator it = server.getChannel(cmd[1]).getUsers().begin(); it != server.getChannel(cmd[1]).getUsers().end(); ++it) {
            write(it->getFd(), message.c_str(), message.length());
        }
        // write(fd, message.c_str(), message.length());
    }
    else if (cmd[0] == "NICK")
    {
        server.getUser(fd).setNickName(cmd[1]);
        std::cout << "nickname : " << server.getUser(fd).getNickName() << std::endl;
    }
    else if (cmd[0] == "USER")
    {
        server.getUser(fd).setUserName(cmd[1]);
        std::cout << "username : " << server.getUser(fd).getUserName() << std::endl;
        string s = ":" + server.getUser(fd).getNickName() + " 001 aaa\n";
        n = write(fd, s.c_str(), s.length());
    }
    else if (cmd[0] == "JOIN") // channel join
    {
        server.getChannel(cmd[1]).addUser(server.getUser(fd));
    }
    else if (cmd[0] == "PRIVMSG") // send msg
    {
        string s = "";
        for (int i = 0; i < cmd.size(); i++)
        {
            s += cmd[i];
            if (i != cmd.size() - 1)
                s += " ";
        }
        s += "\n";
        if (cmd[1].c_str()[0] == '#')
        { 
            std::cout << "user size: " << server.getChannel(cmd[1]).getUsers().size() << std::endl;
            for (int i = 0; i < server.getChannel(cmd[1]).getUsers().size(); i++)
            {
                if (server.getChannel(cmd[1]).getUsers()[i].getFd() == fd)
                    continue ;
                write(server.getChannel(cmd[1]).getUsers()[i].getFd(), s.c_str(), s.length());
            }
        }
    }
    else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
    {

    }
    else if (cmd[0] == "QUIT")
    {
        server.getUsers2().erase(fd);
        close(fd);
    }
    memset(server.getUser(fd).getBuf(), 0, sizeof(char) * 1024);
    return 0;
}