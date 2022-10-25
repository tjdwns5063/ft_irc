// #include "server.hpp"
#include "Translator.hpp"



int request(Server &server, int fd, vector<std::string> cmd)
{
    int n = 0;

    if (cmd[0] == "CAP")
    {
        server.addEvents(fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
        if (n < 0)
            return 1;
    }
    else if (cmd[0] == "NICK")
    {
        server.getUser(fd).setNickName(cmd[1]);
        std::cout << "nickname : " << server.getUser(fd).getNickName() << std::endl;
    }
    else if (cmd[0] == "USER")
    {
        server.getUser(fd).setUserName(cmd[1]);
        std::cout << "username : " << server.getUser(fd).getUserName() << std::endl;
        string s = ":" + server.getUser(fd).getNickName() + " 001 aaa\n";
        n = write(fd, s.c_str(), s.length());
    }
    else if (cmd[0] == "JOIN") // channel join
    {
        server.getChannel(cmd[1]).addUser(server.getUser(fd));
    }
    else if (cmd[0] == "PRIVMSG") // send msg
    {
        string s = "";
        for (int i = 0; i < cmd.size(); i++)
        {
            s += cmd[i];
            if (i != cmd.size() - 1)
                s += " ";
        }
        s += "\n";
        if (cmd[1].c_str()[0] == '#')
        { 
            std::cout << "user size: " << server.getChannel(cmd[1]).getUsers().size() << std::endl;
            for (int i = 0; i < server.getChannel(cmd[1]).getUsers().size(); i++)
            {
                if (server.getChannel(cmd[1]).getUsers()[i].getFd() == fd)
                    continue ;
                write(server.getChannel(cmd[1]).getUsers()[i].getFd(), s.c_str(), s.length());
            }
        }
    }
    else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
    {

    }
    else if (cmd[0] == "QUIT")
    {
        close(fd);
    }
    return 0;
}