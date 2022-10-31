#include "Translator.hpp"

std::string translateResult(const std::string& nickName, ResultCode result, std::vector<std::string> cmd) {
    // :scarlet.irc.ozinger.org 001 seongjki :Welcome to the Ozinger IRC Network seongjki!seongjki@121.135.181.35
    // <network name> <number reply> <nickname> <:message>
    std::string message;

    switch (result)
    {
    case RPL_YOUREOPER:
        message = ":localhost 381 " + nickName + " :You are now an IRC operator\n";
        break ;
    
    case ERR_NOSUCHNICK:
        message = ":localhost 401 " + nickName + " " + cmd[1] + " :No such nick\n";
        break ;

    case ERR_NOSUCHCHANNEL:
        message = ":localhost 403 " + nickName + " " + cmd[1] + " :No such channel\n";
        break ;
    
    case ERR_CANNOTSENDTOCHAN:
        message = ":localhost 404 " + nickName + " " + cmd[1] + " :Cannot send to channel\n";

    case ERR_ERRONEUSNICKNAME:
        message = ":localhost 432 " + nickName + "  " + cmd[1] + " :Erroneus nickname\n";
        break ;
    
    case ERR_NICKNAMEINUSE:
        message = ":localhost 433 " + nickName + " " + cmd[1] + " :Nickname is already in use\n";
        break ;

    case ERR_USERNOTINCHANNEL:
        message = ":localhost 441 " + nickName + " " + cmd[2] + " :They aren't on that channel\n";
        break ;
    
    case ERR_NOTONCHANNEL:
        message = ":localhost 442 " + nickName + " " + cmd[1] + " :You're not on that channel\n";
        break ;
    
    case ERR_NEEDMOREPARAMS:
        message = ":localhost 461 " + nickName + " " + cmd[0] + " :Not enough parameters\n";
        break ;
    
    case ERR_ALREADYREGISTERED:
        message = ":localhost 462 " + nickName + " :You may not reregister\n";
        break ;
    
    case ERR_PASSWDMISMATCH:
        message = ":localhost 464 " + nickName + " :Password incorrect\n";
        break ;
    
    case ERR_NOPRIVILEGES:
        message = ":localhost 481 " + nickName + " :Permission Denied- You're not an IRC operator\n";
        break ;
    
    case ERR_CHANOPRIVSNEEDED:
        message = ":localhost 482 " + nickName + " " + cmd[1] + " :You're not channel operator\n";
        break ;
    
    default:
        break ;
    }
    // std::cout << "asdfmessage: " << message << "\n";
    return message;
}

std::string makePassMesaage(Server &server, vector<std::string>& cmd, int& fd) {
    std::string message;
    const std::string& pw = server.getPassword();

    if (cmd.size() < 2) {
        message = ":localhost 461 " + server.getUser(fd).getNickName() + " :Not enough parameters\n";
    } else if (pw != cmd[1]) {
        message = ":localhost 464 " + server.getUser(fd).getNickName() + " :Password incorrect\n";
    }
    if (server.getUsers().find(fd)->second.getUserName() != "") {
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

int request(Server &server, int fd, std::string s)
{
    vector<std::string> cmd = split(s, ' ');

    // translateResult(server.getUser(fd).getNickName(), DEFAULT, cmd);
    for (int i = 0 ; i < (int)cmd.size(); i++)
    {
        std::cout << "cmd" << i << ": " << cmd[i] << std::endl;
    }
    if (cmd[0] == "PASS")
    {
        cmd_pass(server, fd, cmd);
    }
    else if (cmd[0] == "NICK")
    {
        cmd_nick(server, fd, s, cmd);
    }
    else if (cmd[0] == "USER")
    {
        cmd_user(server, fd, s, cmd);
    }
    else if (cmd[0] == "JOIN") // channel join
    {
        cmd_join(server, fd, cmd);
    }
    else if (cmd[0] == "PRIVMSG") // send msg
    {
        cmd_privmsg(server, fd, s, cmd);
    }
    else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
    {
        cmd_part(server, fd, s, cmd);
    }
    else if (cmd[0] == "QUIT")
    {
        cmd_quit(server, fd);
    }
    else {
       memset(server.getUser(fd).getBuf(), 0, BUF_SIZE);
    }
    while (!cmd.empty())
        cmd.pop_back();
    // memset(server.getUser(fd).getBuf(), 0, sizeof(char) * 1024);
    return 0;
}

// int request(Server &server, int fd, vector<std::string> cmd)
// int request(Server &server, int fd, std::string s)
// {
//     int n = 0;
//     vector<std::string> cmd = split(s, ' ');

//     for (int i = 0 ; i < (int)cmd.size(); i++)
//     {
//         std::cout << "cmd" << i << ": " << cmd[i] << std::endl;
//     }
//     if (cmd[0] == "PASS")
//     {
//         std::string message = makePassMesaage(server, cmd, fd);
//         write(fd, message.c_str(), message.length());
//     }
//     else if (cmd[0] == "OPER")
//     {
//         std::string message = makeOperMessage(server, cmd, fd);
//         write(fd, message.c_str(), message.length());
//     }
//     else if (cmd[0] == "KICK")
//     {
//         int result = makeKickMessage(server, cmd, fd);
//         std::string translateResult(result, cmd[0]);
//         std::cout << "size: " << server.getChannel(cmd[1]).getUsers().size() << '\n';
//         for (vector<User>::const_iterator it = server.getChannel(cmd[1]).getUsers().begin(); it != server.getChannel(cmd[1]).getUsers().end(); ++it) {
//             write(it->getFd(), message.c_str(), message.length());
//         }
//         // write(fd, message.c_str(), message.length());
//         cmd_pass(server, fd, cmd);
//     }
//     else if (cmd[0] == "NICK")
//     {
//         cmd_nick(server, fd, s, cmd);
//     }
//     else if (cmd[0] == "USER")
//     {
//         cmd_user(server, fd, s, cmd);
//     }
//     else if (cmd[0] == "JOIN") // channel join
//     {
//         cmd_join(server, fd, cmd);
//     }
//     else if (cmd[0] == "PRIVMSG") // send msg
//     {
//         cmd_privmsg(server, fd, s, cmd);
//     }
//     else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
//     {
//         cmd_part(server, fd, s, cmd);
//     }
//     else if (cmd[0] == "QUIT")
//     {
//         cmd_quit(server, fd);
//     }
//     memset(server.getUser(fd).getBuf(), 0, sizeof(char) * 1024);
//     return 0;
// }