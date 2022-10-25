#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include "User.hpp"

class Channel {
private:
    std::vector<User> users;
    std::vector<User> op;
public:
    // Channel(std::string channelName, User user);
    Channel();
    ~Channel();
    Channel(const Channel& channel);
    Channel& operator=(const Channel& channel);
    void addUser(User &user);
    const std::vector<User> &getUsers();
};

#endif