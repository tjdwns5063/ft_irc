#include "Command.hpp"

void send_all(Server &server, std::string s)
{
	std::map<int, User*> &users = server.getUsers();

	for (std::map<int, User*>::iterator it = users.begin(); it != users.end(); it++)
	{
        server.getUser(it->second->getFd())->setBuf(s);
        server.addEvents(it->second->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	}
}

void send_channel(Server &server, Channel &channel, string s)
{
    std::vector<User*> users = channel.getUsers();
    for (unsigned long i = 0 ; i < users.size(); i++)
    {
        server.getUser(users[i]->getFd())->setBuf(s);
        server.addEvents(users[i]->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
}

void send_allChannel(Server &server, User &user, string s)
{
    std::vector<Channel*> &channels = user.getChannels();

    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (!(*it)->getName().empty())
            send_channel(server, *server.getChannel((*it)->getName()), s);
    }
}

void send_excludeme(Server &server, Channel &channel, User &user, string s)
{
    for (std::vector<User*>::iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); it++)
    {
        if ((*it)->getFd() == user.getFd())
            continue ;
        server.getUser((*it)->getFd())->setBuf(s);
        server.addEvents((*it)->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
    user.clearBuf();
}

void send_fd(Server &server, int fd, string s)
{
    server.getUser(fd)->setBuf(s);
    server.addEvents(fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
}
