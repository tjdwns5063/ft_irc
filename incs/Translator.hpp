#ifndef TRANSLATOR_HPP
# define TRANSLATOR_HPP

#include "../incs/Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>

class Translator {
private:
    std::string translateCapMsg(std::string& msg);
    std::string translateUserMsg(std::string& msg);
    std::string translateNickMsg(std::string& msg);
public:
    Translator();
    ~Translator();
    std::string translate(std::string& msg);
};
class Server;

int request(Server &server, int fd, std::vector<std::string> cmd);


#endif