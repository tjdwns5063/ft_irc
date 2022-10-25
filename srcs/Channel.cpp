#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const Channel& channel) {
    this->users = channel.users;
    this->op = channel.op;
}

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