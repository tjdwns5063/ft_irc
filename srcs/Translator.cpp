#include "Translator.hpp"

int request(Server &server, int fd, std::string s)
{
    int n = 0;
    vector<std::string> cmd = split(s, ' ');

    for (int i = 0 ; i < (int)cmd.size(); i++)
    {
        std::cout << "cmd" << i << ": " << cmd[i] << std::endl;
    }
    if (cmd[0] == "PASS")
    {
        cmd_pass(server, fd, cmd);
    }
    else if (cmd[0] == "NICK")
    {
        cmd_nick(server, fd, s, cmd);
    }
    else if (cmd[0] == "USER")
    {
        cmd_user(server, fd, s, cmd);
    }
    else if (cmd[0] == "JOIN") // channel join
    {
        cmd_join(server, fd, cmd);
    }
    else if (cmd[0] == "PRIVMSG") // send msg
    {
        cmd_privmsg(server, fd, s, cmd);
    }
    else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
    {
        cmd_part(server, fd, s, cmd);
    }
    else if (cmd[0] == "QUIT")
    {
        cmd_quit(server, fd);
    }
    memset(server.getUser(fd).getBuf(), 0, sizeof(char) * 1024);
    return 0;
}