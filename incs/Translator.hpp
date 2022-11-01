#ifndef TRANSLATOR_HPP
# define TRANSLATOR_HPP

#include "Server.hpp"
#include "Command.hpp"

class Server;

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

int request(Server &server, int fd, std::string s);
std::string translateResult(const std::string& nickName, ResultCode result, std::vector<std::string> cmd);

#endif