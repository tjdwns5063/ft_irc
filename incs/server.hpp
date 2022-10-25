#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/event.h>
#include <vector>
#include <map>
#include <stdio.h>
#include "User.hpp"
#include "Translator.hpp"

using namespace std;

class Server {
private:
    int status;
    int port;
    std::string password;
    std::map<int, User> users;
    struct sockaddr_in server_addr;
    int server_sock;
    int kqueue_fd;
    std::vector<struct kevent> changed;
    struct kevent event_list[10];
    char buf[1024];

private:
    int errorFlagLogic(struct kevent* currEvent);
    int readFlagLogic(struct kevent* currEvent, int& writeFlag);
    int writeFlagLogic(struct kevent* currEvent, int& writeFlag);
    void handleWriteFlag(int& writeFlag);

public:
    Server(int port, std::string password);
    ~Server();
    int getStatus() const;
    int makeServerSock();
    void addEvents(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
    void run();
    int connectClient();
    int checkEvent(int newEvent);
    User &getUser(int n);
};

std::vector<std::string> split(std::string input, char delimiter);


#endif