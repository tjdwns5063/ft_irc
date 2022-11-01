#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"
// #include "User.hpp"

class Server;

// void send_all(Server &server, std::string s);
// void send_allChannel(User &user, std::string s);
// void send_channel(Channel &channel, User *user, std::string s);
// void send_channel_all(Channel &channel, std::string s);

void cmd_nick(Server &server, int fd, std::vector<std::string> cmd);

void cmd_user(Server &server, int fd, std::vector<std::string> cmd);
void cmd_join(Server &server, int fd, std::vector<std::string> cmd);
void cmd_privmsg(Server &server, int fd, std::vector<std::string> cmd);

void cmd_part(Server &server, int fd, std::vector<std::string> cmd);
void cmd_quit(Server &server, int fd);

void cmd_kick(Server &server, int fd, std::vector<std::string>& cmd);
void cmd_oper(Server &server, int fd, std::vector<std::string>& cmd);
void cmd_pass(Server &server, int fd, std::vector<std::string>& cmd);

void cmd_kill(Server &server, int fd, std::vector<std::string>& cmd);
void cmd_unknown(Server& server, int fd, std::vector<std::string>& cmd);

#endif
