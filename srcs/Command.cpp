#include "Command.hpp"

void send_all(Server &server, string s)
{
	std::map<int, User> &users = server.getUsers();

	for (std::map<int, User>::iterator it = users.begin(); it != users.end(); it++)
	{
        server.getUser(it->second.getFd()).setBuf(s);
        server.addEvents(it->second.getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	}
}

void send_allChannel(Server &server, User &user, string s)
{
    std::vector<Channel> &channels = user.getChannels();

    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::vector<User> &users = it->getUsers();
        for (std::vector<User>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            server.getUser(it2->getFd()).setBuf(s);

            server.addEvents(it2->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
            // write(it2->getFd(), s.c_str(), s.length());
        }
    }
}

// void send_channel(Server &server, Channel &channel, User *user, string s)
void send_channel(Server &server, Channel &channel, string s)
{
    // if (!user)
    {
        for (std::vector<User>::iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++)
        {
            server.getUser(it->getFd()).setBuf(s);
            // write(it->getFd(), s.c_str(), s.length());
            server.addEvents(it->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
        }
    }
    // else
    // {
    //     for (std::vector<User>::iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++)
    //     {
    //         if (it->getFd() == user->getFd())
    //             continue ;
    //         server.getUser(it->getFd()).setBuf(s);
    //         server.addEvents(it->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    //     }
    // }

}

void cmd_pass(Server &server, int fd, std::vector<std::string> cmd)
{
    if (cmd.size() < 2)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    const std::string& pw = server.getPassword();
    if (pw != cmd[1]) {
        write(fd, ":seongjki Password incorrect\n", strlen(":seongjki Password incorrect\n") + 1);
    }        
}

void cmd_nick(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 2)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    User &user = server.getUser(fd);
    if (user.getNickName() == cmd[1])
        return ;
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s + "\n";
	user.setNickName(cmd[1]);
    std::cout << "nick response: " << s << std::endl;
    send_allChannel(server, user, s);
    std::cout << "nickname : " << user.getNickName() << std::endl;
}

void cmd_user(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 5)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    if (!server.getUser(fd).getUserName().empty())
        return ;
    server.getUser(fd).setUserName(cmd[1] + "@" + cmd[3]);
    std::cout << "username : " << server.getUser(fd).getUserName() << std::endl;
    s = ":" + server.getUser(fd).getNickName() + " 001 " + server.getUser(fd).getUserName() + "\n";
    // s = ":001 " + server.getUser(fd).getNickName() + "\n";
    server.getUser(fd).setBuf(s);


    server.addEvents(fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    // write(fd, s.c_str(), s.length());
}

void cmd_join(Server &server, int fd, std::vector<std::string> cmd)
{
    if (cmd.size() < 2)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        channel.addUser(server.getUser(fd));
        server.getUser(fd).addChannel(channel);
    }
    else {
        // error 403
        translateResult(server.getUser(fd).getNickName(), ERR_NOSUCHCHANNEL, cmd);
    }
}

void cmd_privmsg(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 3)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    User &user = server.getUser(fd);
    s = ":" + user.getNickName() + "!" + user.getUserName() + " " + s; 
    s += "\n";
    if (cmd[1].c_str()[0] == '#')
    {
        Channel &channel = server.getChannel(cmd[1]);
        std::vector<User> &users = channel.getUsers();
        std::cout << "user size: " << users.size() << std::endl;
        if (channel.chkUser(fd) == false)
        {
            //error 404
            translateResult(user.getNickName(), ERR_CANNOTSENDTOCHAN, cmd);
            return ;
        }
        // send_channel(server, channel, &user, s);
        send_channel(server, channel, s);
    }
    else
    {
        //401
        translateResult(server.getUser(fd).getNickName(), ERR_NOSUCHNICK, cmd);
    }

}

void cmd_part(Server &server, int fd, std::string s, std::vector<std::string> cmd)
{
    if (cmd.size() < 2)
        translateResult(server.getUser(fd).getNickName(), ERR_NEEDMOREPARAMS, cmd);
    Channel &channel = server.getChannel(cmd[1]);
    s = ":" + server.getUser(fd).getNickName() + "!" + server.getUser(fd).getUserName() + " " + s + "\n"; 
    std::cout << s << std::endl;
    send_channel(server, channel, s);
    channel.removeUser(server.getUser(fd));
    server.getUser(fd).leaveChannel(channel);
    // send_channel(server, channel, &server.getUser(fd), s);
}

void cmd_quit(Server &server, int fd)
{
    server.getUsers().erase(fd);
    close(fd);
}
