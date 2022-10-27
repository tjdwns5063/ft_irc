#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"

void cmd_pass(Server &server, int fd, vector<std::string> cmd);
void cmd_nick(Server &server, int fd, std::string s, vector<std::string> cmd);

void cmd_user(Server &server, int fd, std::string s, vector<std::string> cmd);
void cmd_join(Server &server, int fd, vector<std::string> cmd);
void cmd_privmsg(Server &server, int fd, std::string s, vector<std::string> cmd);

void cmd_part(Server &server, int fd, std::string s, vector<std::string> cmd);
void cmd_quit(Server &server, int fd);

#endif