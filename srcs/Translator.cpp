#include "Translator.hpp"
#include "Command.hpp"
#include "ICommand.hpp"

std::string Translator::translateResult(const std::string& nickName, int result, std::vector<std::string> cmd) {
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

std::string Translator::translateSuccess(const std::string& nickName, std::vector<std::string>& cmd, ICommand& command) {
    std::string message;
    
    switch (command.getType())
    {
    case ICommand::PASS :
        message = ":localhost :Password is Correct\n";
        break ;
    case ICommand::NICK :
        message = ":" + nickName + " NICK " + cmd[1] + "\n";
        break ;
    case ICommand::USER :
        message = ":" + nickName + " 001 " + nickName + "\n";
        break ;
    case ICommand::JOIN :
        message = ":" + nickName + " JOIN " + ":" + cmd[1] + "\n";
        break ;
    case ICommand::PRIVMSG :
        message = ":" + nickName + " " + cmd[0] + " " + cmd[1] + " :";
        for (unsigned long i = 2; i < cmd.size(); ++i) {
            message += cmd[i];
            message += " ";
        }
        message += "\n";
        break ;
    case ICommand::PART :
        message = ":" + nickName + " PART " + cmd[1] + "\n";
        break ;
    case ICommand::QUIT :
        message = ":" + nickName + " QUIT :Quit Bye Bye\n";
        break ;
    case ICommand::KICK :
        message = ":" + nickName + " KICK " + cmd[1] + " " + cmd[2];
        if (cmd.size() > 3)
            message += (" :" + cmd[3] + "\n");
        else
            message += (" :" + nickName + "\n");
        break ;
    case ICommand::OPER :
        message = ":localhost 381 " + cmd[1] + " :You are now an IRC operator\n";
        break ;
    case ICommand::KILL :
        message = ":" + nickName + " KILL " + cmd[1] + " :" + nickName + "\n";
        break ;
    case ICommand::PONG :
        message = "PONG " + cmd[1] + "\n";
        break ;
    default:
        break;
    }
    return message;
}
