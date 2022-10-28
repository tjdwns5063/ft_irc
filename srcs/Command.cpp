#include "Command.hpp"

void send_all(Server &server, std::string s)
{
	std::map<int, User> &users = server.getUsers();

	for (std::map<int, User>::iterator it = users.begin(); it != users.end(); it++)
	{
		write(it->second.getFd(), s.c_str(), s.length());
	}
}

void send_allChannel(User &user, std::string s)
{
    std::vector<Channel> &channels = user.getChannels();

    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::vector<User> &users = it->getUsers();
        for (std::vector<User>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            write(it2->getFd(), s.c_str(), s.length());
        }
    }
}

void send_channel(Channel &channel, User *user, std::string s)
{
    if (!user)
    {
        for (std::vector<User>::iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++)
        {
            write(it->getFd(), s.c_str(), s.length());
        }
    }
    else
    {
        for (std::vector<User>::iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++)
        {
            if (it->getFd() == user->getFd())
                continue ;
            write(it->getFd(), s.c_str(), s.length());
        }
    }
}

void send_channel_all(Channel &channel, std::string s)
{
    vector<User>& users = channel.getUsers();

    for (int i = 0; i < users.size(); ++i) {
        write(users[i].getFd(), s.c_str(), s.length());
    }
}


void cmd_pass(Server &server, int fd, std::vector<std::string> cmd)
{
    const std::string& pw = server.getPassword();
    if (pw != cmd[1]) {
        write(fd, ":seongjki Password incorrect\n", strlen(":seongjki Password incorrect\n") + 1);
    }        
}

// void write_err(int fd, int errno)
// {

// }

void cmd_nick(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    User &user = server.getUser(fd);
    // s = ":" + user.getNickName() + " NICK " + cmd[1] + "\n";
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s + "\n";
	user.setNickName(cmd[1]);
    std::cout << "nick response: " << s << std::endl;
    // write(fd, s.c_str(), s.length());
    send_allChannel(user, s);
    std::cout << "nickname : " << user.getNickName() << std::endl;
}

void cmd_user(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (!server.getUser(fd).getUserName().empty())
        return ;
    server.getUser(fd).setUserName(cmd[1] + "@" + cmd[3]);
    std::cout << "username : " << server.getUser(fd).getUserName() << std::endl;
    s = ":" + server.getUser(fd).getNickName() + " 001 " + server.getUser(fd).getNickName() + "\n";
    // s = ":001 " + server.getUser(fd).getNickName() + "\n";
    write(fd, s.c_str(), s.length());
}

void cmd_join(Server &server, int fd, std::vector<std::string> cmd)
{
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        channel.addUser(server.getUser(fd));
        server.getUser(fd).addChannel(channel);
        // server.getChannel(cmd[1]).addUser(server.getUser(fd));
        // server.getUser(fd).addChannel(server.getChannel(cmd[1]));
    }
}

void cmd_privmsg(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    User &user = server.getUser(fd);
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s; 
    s += "\n";
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        std::vector<User> &users = channel.getUsers();
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

void cmd_part(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    s = ":" + server.getUser(fd).getNickName() + "!" + server.getUser(fd).getUserName() + " " + s + "\n"; 
    std::cout << s << std::endl;
    server.getChannel(cmd[1]).deleteUser(server.getUser(fd));
    server.getUser(fd).leaveChannel(server.getChannel(cmd[1]));
    write(fd, s.c_str(), s.length());
}

void cmd_quit(Server &server, int fd)
{
    server.getUsers().erase(fd);
    close(fd);
}

void cmd_kick(std::string& message)
{
    
}
