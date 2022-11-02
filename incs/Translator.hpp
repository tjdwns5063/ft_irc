#ifndef TRANSLATOR_HPP
# define TRANSLATOR_HPP

#include "Server.hpp"
#include "Command.hpp"

class Server;
class ICommand;

class Translator {
public:
    enum ResultCode {
    DEFAULT = 0,
    
    RPL_YOUREOPER = 381,

    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_UNKNOWNCOMMAND = 421,
    
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,

    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,

    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTERED = 462,
    ERR_PASSWDMISMATCH = 464,

    ERR_NOPRIVILEGES = 481,
    ERR_CHANOPRIVSNEEDED = 482
    };

    std::string translateResult(const std::string& nickName, int resultCode, std::vector<std::string> cmd);
    std::string translateSuccess(const std::string& nickName, std::vector<std::string>& cmd, ICommand& command);
};

int request(Server &server, int fd, std::string s);

#endif