#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"
#include "User.hpp"

class Server;

void cmd_pass(Server &server, int fd, std::vector<std::string> cmd);
void cmd_nick(Server &server, int fd, std::string s, std::vector<std::string> cmd);

void cmd_user(Server &server, int fd, std::string s, std::vector<std::string> cmd);
void cmd_join(Server &server, int fd, std::vector<std::string> cmd);
void cmd_privmsg(Server &server, int fd, std::string s, std::vector<std::string> cmd);

void cmd_part(Server &server, int fd, std::string s, std::vector<std::string> cmd);
void cmd_quit(Server &server, int fd);

#endif
