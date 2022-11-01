#include "User.hpp"

User::User(): fd(-1), userName(""), nickName(""), op(0), buf(new char[BUF_SIZE]), killed(false) {
    memset(buf, 0, sizeof(char) * BUF_SIZE);
}

User::User(int fd): fd(fd), userName(""), nickName(""), op(0), buf(new char[BUF_SIZE]), killed(false) {
    memset(buf, 0, sizeof(char) * BUF_SIZE);
}

User::User(int fd, std::string userName, std::string nickName, bool op)
    : fd(fd), userName(userName), nickName(nickName), op(op), buf(new char[BUF_SIZE]), killed(false) {
        memset(buf, 0, sizeof(char) * BUF_SIZE);
    }

User::User(const User& user)
    : fd(user.fd), userName(user.userName), nickName(user.nickName), op(user.op), buf(new char[BUF_SIZE]), killed(false) {
        memset(buf, 0, sizeof(char) * BUF_SIZE);
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
    this->op = user.op;
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

bool User::getOp() const {
    return (this->op);
}

bool User::getKilled() const {
    return killed;
}

void User::setUserName(std::string userName) {
    this->userName = userName;
}

void User::setNickName(std::string nickName) {
    this->nickName = nickName;
}

void User::setOp(bool op) {
    this->op = op;
}

void User::setKilled(bool killed) {
    this->killed = killed;
}

bool operator==(const User& lhs, const User& rhs) {
    return lhs.getFd() == rhs.getFd() &&
        lhs.getNickName() == rhs.getNickName() &&
        lhs.getOp() == rhs.getOp() &&
        lhs.getUserName() == rhs.getUserName();
}

std::vector<Channel> &User::getChannels() {
    return channels;
}

void User::addChannel(Channel &channel) {
    channels.push_back(channel);
}


void User::leaveChannel(Channel &channel) {

    for (int i = 0; i < (int) channels.size(); i++)
    {
        if (channels[i] == channel) {
            channels.erase(channels.begin() + i);
            return ;
        }
    }
}