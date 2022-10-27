#ifndef USER_HPP
# define USER_HPP
#include "Channel.hpp"

class Channel;

class User {
private:
    int fd;
    std::string userName;
    std::string nickName;
    std::vector<Channel> channels;
    bool op;
    char *buf;

public:
    User();
    User(int fd);
    User(int fd, std::string userName, std::string nickName, bool op);
    User(const User& user);
    ~User();
    User& operator=(const User& user);
    int getFd() const;
    char* getBuf();
    const std::string getUserName() const;
    const std::string getNickName() const;
    bool getOp() const;
    void setUserName(std::string userName);
    void setNickName(std::string nickName);
    void setOp(bool op);
    std::vector<Channel> &getChannels();
    void addChannel(Channel &channel);
    void leaveChannel(Channel &channel);
};

#endif