// #include "server.hpp"
#include "Translator.hpp"

int request(Server &server, int fd, vector<std::string> cmd)
{
    int n = 0;

    for (int i = 0 ; i < cmd.size(); i++)
    {
        std::cout << "cmd" << i << ": " << cmd[i] << std::endl;
    }
    if (cmd[0] == "PASS")
    {
        const std::string& pw = server.getPassword();
        if (pw != cmd[1]) {
            write(fd, ":seongjki Password incorrect\n", strlen(":seongjki Password incorrect\n" + 1));
        }        
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
        n = write(fd, ":seongjki 001 seongjki\n", strlen(":seongjki 001 seongjki\n") + 1);
        if (n < 0)
            return 1;
    }
    else if (cmd[0] == "JOIN") // channel join
        ;
    else if (cmd[0] == "PRIVMSG") // send msg
    {

    }
    else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
    {

    }
    else if (cmd[0] == "QUIT")
    {
        server.getUsers().erase(fd);
        close(fd);
    }
    memset(server.getUser(fd).getBuf(), 0, sizeof(char) * 1024);
    return 0;
}