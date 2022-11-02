#ifndef USER_HPP
# define USER_HPP
#include "Channel.hpp"

#define BUF_SIZE 4096

class Channel;

enum FlagEnum {
  OP,
  KILLED,
  PASSED,
  REGISTERD  
};

class User {
private:
    int fd;
    std::string userName;
    std::string nickName;
    std::vector<Channel*> channels;
    char *buf;
    bool flags[4]; // 0: OP 1: Killed 2: Passed 3: Registerd;

public:
    User();
    User(int fd);
    User(int fd, std::string userName, std::string nickName);
    User(const User& user);
    ~User();
    User& operator=(const User& user);
    int getFd() const;
    char* getBuf();
    void setBuf(std::string s);
    void setBuf(char *s);
    void clearBuf();
    const std::string getUserName() const;
    const std::string getNickName() const;
    bool getFlag(int idx) const;
    void setUserName(std::string userName);
    void setNickName(std::string nickName);
    void setFlag(int idx, bool flag);
    std::vector<Channel*> &getChannels();
    void addChannel(Channel &channel);
    void leaveChannel(Channel &channel);
};

bool operator==(const User& lhs, const User& rhs);

#endif