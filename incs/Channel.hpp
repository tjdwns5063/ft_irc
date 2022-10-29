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
    std::vector<User>::iterator searchUser(std::string& userName);
    // void addUser(User &user);
    bool removeUser(std::string& userName);
    // const std::vector<User> &getUsers() const;
    int operator==(const Channel& channel);
    void addUser(User &user);
    void removeUser(User &user);
    std::vector<User> &getUsers();
    bool chkUser(int fd);
};

#endif