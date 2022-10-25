#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include "User.hpp"

class Channel {
private:
    std::map<std::string, std::vector<User> > info;

public:
    // Channel(std::string channelName, User user);
    Channel();
    ~Channel();
    Channel(const Channel& channel);
    Channel& operator=(const Channel& channel);
    const std::map<std::string, std::vector<User> >& getInfo() const;
    void addChannel(std::string name, User user);
};

#endif