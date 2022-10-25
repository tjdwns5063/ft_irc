#ifndef TRANSLATOR_HPP
# define TRANSLATOR_HPP

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

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