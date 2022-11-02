#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"
// #include "User.hpp"

class Server;

// void send_all(Server &server, std::string s);
// void send_allChannel(User &user, std::string s);
// void send_channel(Channel &channel, User *user, std::string s);
// void send_channel_all(Channel &channel, std::string s);
void send_all(Server &server, std::string s);
void send_channel(Server &server, Channel &channel, std::string s);
void send_allChannel(Server &server, User &user, std::string s);
void send_excludeme(Server &server, Channel &channel, User &user, std::string s);
void send_fd(Server &server, int fd, std::string s);

#endif
