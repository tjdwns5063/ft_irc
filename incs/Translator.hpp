#ifndef TRANSLATOR_HPP
# define TRANSLATOR_HPP

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

#endif