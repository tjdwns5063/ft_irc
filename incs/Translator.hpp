#ifndef TRANSLATOR_HPP
# define TRANSLATOR_HPP

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Server;

int request(Server &server, int fd, std::vector<std::string> cmd);
#endif