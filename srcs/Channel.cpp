#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const Channel& channel) {
    this->users = channel.users;
    this->op = channel.op;
}

Channel& Channel::operator=(const Channel& channel) {
    if (this == &channel)
        return *this;
    this->users = channel.users;
    this->op = channel.op;
    return *this;
}

std::vector<User> &Channel::getUsers() {
    return users;
}

void Channel::addUser(User &user) {
    users.push_back(user);
}

void Channel::deleteUser(User &user) {
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->getFd() == user.getFd())
		{
			std::cout << "Delete user\n";
			users.erase(it);
			break ;
		}
	}
}

int Channel::chkUser(int fd)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); it++)
	{
        if (it->getFd() == fd)
        {
            return 1;
        }
	}
	return 0;
}