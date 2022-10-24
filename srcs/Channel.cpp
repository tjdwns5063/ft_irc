#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const Channel& channel) {
    this->info = channel.info;
}

Channel& Channel::operator=(const Channel& channel) {
    if (this == &channel)
        return *this;
    this->info = channel.info;
    return *this;
}

const std::map<std::string, std::vector<User> >& Channel::getInfo() const {
    return info;
}

void Channel::addChannel(std::string name, User user) {
    info[name].push_back(user);
}