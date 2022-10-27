#include "Command.hpp"

void send_all(Server &server, string s)
{
	std::map<int, User> users = server.getUsers();

	for (std::map<int, User>::iterator it = users.begin(); it != users.end(); it++)
	{
		write(it->second.getFd(), s.c_str(), s.length());
	}
}

void cmd_pass(Server &server, int fd, vector<std::string> cmd)
{
    const std::string& pw = server.getPassword();
    if (pw != cmd[1]) {
        write(fd, ":seongjki Password incorrect\n", strlen(":seongjki Password incorrect\n") + 1);
    }        

}
void cmd_nick(Server &server, int fd, std::string s, vector<std::string> cmd)
{
    s = ":"+server.getUser(fd).getNickName() + " NICK " + cmd[1] + "\n";
	server.getUser(fd).setNickName(cmd[1]);
	write(fd, s.c_str(), s.length());
    std::cout << "nickname : " << server.getUser(fd).getNickName() << std::endl;
}

void cmd_user(Server &server, int fd, std::string s, vector<std::string> cmd)
{
    server.getUser(fd).setUserName(cmd[1]);
    std::cout << "username : " << server.getUser(fd).getUserName() << std::endl;
    s = ":" + server.getUser(fd).getNickName() + " 001 " + server.getUser(fd).getNickName() + "\n";
    // s = ":001 " + server.getUser(fd).getNickName() + "\n";
    write(fd, s.c_str(), s.length());
}

void cmd_join(Server &server, int fd, vector<std::string> cmd)
{
    server.getChannel(cmd[1]).addUser(server.getUser(fd));
}

void cmd_privmsg(Server &server, int fd, std::string s, vector<std::string> cmd)
{
    s += "\n";
    if (cmd[1].c_str()[0] == '#')
    {
        Channel channel = server.getChannel(cmd[1]);
        std::vector<User> users = channel.getUsers();
        std::cout << "user size: " << users.size() << std::endl;
        if (channel.chkUser(fd) == 0)
        {
            write(1, ":401\n", strlen(":401\n"));
            write(fd, ":401\n", strlen(":401\n"));
            return ;
        }
        for (int i = 0; i < (int) users.size(); i++)
        {
            if (users[i].getFd() == fd)
                continue ;
            write(users[i].getFd(), s.c_str(), s.length());
        }
    }

}

void cmd_part(Server &server, int fd, std::string s, vector<std::string> cmd)
{
    server.getChannel(cmd[1]).deleteUser(server.getUser(fd));
    write(fd, s.c_str(), s.length());
}

void cmd_quit(Server &server, int fd)
{
    server.getUsers2().erase(fd);
    close(fd);
}
