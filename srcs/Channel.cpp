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

const std::vector<User> &Channel::getUsers() {
    return users;
}
void Channel::addUser(User &user) {
    users.push_back(user);
}

std::vector<User>::iterator Channel::searchUser(std::string& userName) {
    for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it) {
        if (it->getUserName() == userName) {
            return it;
        }
    }
    return users.end();
}

bool Channel::removeUser(std::string& userName) {
    std::vector<User>::iterator pos = searchUser(userName);
    if (pos != users.end()) {
        users.erase(pos);
        return true;
    }
    return false;
}