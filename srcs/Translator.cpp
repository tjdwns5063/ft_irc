#include "Translator.hpp"
#include "Command.hpp"

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
        break ;

    case ERR_UNKNOWNCOMMAND:
        message = ":localhost 421 " + cmd[0] + " :Unknown command\n";
        break ;

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
    return message;
}




int request(Server &server, int fd, std::string s)
{
    vector<std::string> cmd = split(s, ' ');

    if (fd < 0)
        return 1;
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
        cmd_nick(server, fd, cmd);
    }
    else if (cmd[0] == "USER")
    {
        cmd_user(server, fd, cmd);
    }
    else if (cmd[0] == "JOIN") // channel join
    {
        cmd_join(server, fd, cmd);
    }
    else if (cmd[0] == "PRIVMSG") // send msg
    {
        cmd_privmsg(server, fd, cmd);
    }
    else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
    {
        cmd_part(server, fd, cmd);
    }
    else if (cmd[0] == "QUIT")
    {
        cmd_quit(server, fd);
    }
    else if (cmd[0] == "KICK")
    {
        cmd_kick(server, fd, cmd);
    }
    else if (cmd[0] == "OPER")
    {
        cmd_oper(server, fd, cmd);
    }
    else if (cmd[0] == "kill")
    {
        cmd_kill(server, fd, cmd);
    }    
    else if (cmd[0] == "PING")
    {
        cmd_ping(server, fd, cmd);
    }
     else {
        cmd_unknown(server, fd, cmd);
    }

    while (!cmd.empty())
        cmd.pop_back();
    // memset(server.getUser(fd).getBuf(), 0, sizeof(char) * 1024);
    return 0;
}
 