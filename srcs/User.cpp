#include "User.hpp"

User::User(): fd(-1), userName(""), nickName(""), buf(new char[BUF_SIZE])  {
    memset(buf, 0, sizeof(char) * BUF_SIZE);
    memset(flags, 0, sizeof(bool) * 4);
}

User::User(int fd): fd(fd), userName(""), nickName(""), buf(new char[BUF_SIZE]) {
    memset(buf, 0, sizeof(char) * BUF_SIZE);
    memset(flags, 0, sizeof(bool) * 4);
}

User::User(int fd, std::string userName, std::string nickName)
    : fd(fd), userName(userName), nickName(nickName), buf(new char[BUF_SIZE]) {
        memset(buf, 0, sizeof(char) * BUF_SIZE);
        memset(flags, 0, sizeof(bool) * 4);
    }

User::User(const User& user)
    : fd(user.fd), userName(user.userName), nickName(user.nickName), buf(new char[BUF_SIZE]) {
        memset(buf, 0, sizeof(char) * BUF_SIZE);
        memset(flags, 0, sizeof(bool) * 4);
    }
 
User::~User() {
    delete[] (buf);
}

User& User::operator=(const User& user) {
    if (this == &user) {
        return (*this);
    }
    this->fd = user.fd;
    this->userName = user.userName;
    this->nickName = user.nickName;
    return (*this);
}

int User::getFd() const {
    return (this->fd);
}

char* User::getBuf() {
    return (this->buf);
}

void User::setBuf(std::string s) {
    int i;
    for (i = 0; i < BUF_SIZE; i++)
        buf[i] = 0;
    for (i = 0; i < (int) s.length(); i++)
    {
        buf[i] = s[i];
    }
    buf[i] = '\0';

}

void User::setBuf(char *s)
{
    int i;
    for (i = 0; i < (int) strlen(s); i++)
    {
        buf[i] = s[i];
    }
    buf[i] = '\0';
}

void User::clearBuf() {
    memset(buf, 0, BUF_SIZE);
}

const std::string User::getUserName() const {
    return (this->userName);
}

const std::string User::getNickName() const {
    return (this->nickName);
}

bool User::getFlag(int idx) const {
    return (this->flags[idx]);
}

void User::setUserName(std::string userName) {
    this->userName = userName;
}

void User::setNickName(std::string nickName) {
    this->nickName = nickName;
}

void User::setFlag(int idx, bool flag) {
    this->flags[idx] = flag;
}

bool operator==(const User& lhs, const User& rhs) {
    return lhs.getFd() == rhs.getFd() &&
        lhs.getNickName() == rhs.getNickName() &&
        lhs.getUserName() == rhs.getUserName();
}
std::vector<Channel*> &User::getChannels() {
    return channels;
}

void User::addChannel(Channel &channel) {
    channels.push_back(&channel);
}


void User::leaveChannel(Channel &channel) {

    for (int i = 0; i < (int) channels.size(); i++)
    {
        if (*channels[i] == channel) {
            channels.erase(channels.begin() + i);
            return ;
        }
    }
}