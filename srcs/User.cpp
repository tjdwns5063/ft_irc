#include "User.hpp"

User::User(): fd(-1), userName(""), nickName(""), op(0), buf(new char[1024]) {
    memset(buf, 0, sizeof(char) * 1024);
}

User::User(int fd): fd(fd), userName(""), nickName(""), op(0), buf(new char[1024]) {
    memset(buf, 0, sizeof(char) * 1024);
}

User::User(int fd, std::string userName, std::string nickName, bool op)
    : fd(fd), userName(userName), nickName(nickName), op(op), buf(new char[1024]) {
        memset(buf, 0, sizeof(char) * 1024);
    }

User::User(const User& user)
    : fd(user.fd), userName(user.userName), nickName(user.nickName), op(user.op), buf(new char[1024]) {
        memset(buf, 0, sizeof(char) * 1024);
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

const std::string User::getUserName() const {
    return (this->userName);
}

const std::string User::getNickName() const {
    return (this->nickName);
}

bool User::getOp() const {
    return (this->op);
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

bool operator==(const User& lhs, const User& rhs) {
    return lhs.getFd() == rhs.getFd() &&
        lhs.getNickName() == rhs.getNickName() &&
        lhs.getOp() == rhs.getOp() &&
        lhs.getUserName() == rhs.getUserName();
}