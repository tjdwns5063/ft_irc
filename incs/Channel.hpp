#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include "User.hpp"
#include <iostream>

class User;
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
    int operator==(const Channel& channel);
    void addUser(User &user);
    void deleteUser(User &user);
    std::vector<User> &getUsers();
    int chkUser(int fd);
};

#endif