#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(std::string s): name(s) {
    
}

Channel::Channel(const Channel& channel) {
    this->users = channel.users;
    this->name = channel.name;
}

Channel& Channel::operator=(const Channel& channel) {
    if (this == &channel)
        return *this;
    this->users = channel.users;
    this->name = channel.name;
    return *this;
}

const std::string &Channel::getName() const {
    return name;
}

bool Channel::operator==(const Channel& channel) {
    return name == channel.getName();
}

std::vector<User*> &Channel::getUsers() {
    return users;
}

void Channel::addUser(User &user) {
    users.push_back(&user);
}

std::vector<User*>::iterator Channel::searchUser(const std::string& nickName) {
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getNickName() == nickName) {
            return it;
        }
    }
    return users.end();
}

bool Channel::removeUser(std::string& userName) {
    std::vector<User*>::iterator pos = searchUser(userName);
    if (pos != users.end()) {
        users.erase(pos);
        return true;
    }
    return false;
}

void Channel::removeUser(User &user) {
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
	{
		if ((*it)->getFd() == user.getFd())
		{
			users.erase(it);
			break ;
		}
	}
}

bool Channel::chkUser(int fd)
{
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
	{
        if ((*it)->getFd() == fd)
        {
            return 1;
        }
	}
	return 0;
}
