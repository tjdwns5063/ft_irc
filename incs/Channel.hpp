#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <cstring>
#include <map>
#include <vector>
#include "User.hpp"
#include <iostream>

class User;
class Channel {
private:
    std::string name;
    std::vector<User*> users;
public:
    // Channel(std::string channelName, User user);
    Channel();
    Channel(std::string s);
    ~Channel();
    Channel(const Channel& channel);
    Channel& operator=(const Channel& channel);
    std::vector<User*>::iterator searchUser(const std::string& userName);
    bool removeUser(std::string& userName);
    const std::string &getName() const;
    // const std::vector<User> &getUsers() const;
    bool operator==(const Channel& channel);
    void addUser(User &user);
    void removeUser(User &user);
    std::vector<User*> &getUsers();
    bool chkUser(int fd);
};

#endif