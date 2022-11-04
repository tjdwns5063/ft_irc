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
#include <queue>
#include <stdio.h>
#include "User.hpp"
#include "Translator.hpp"
#include "Channel.hpp"
using namespace std;

#define BUF_SIZE 4096

class ICommand;
class Translator;

class Server {
private:
    int status;
    int port;
    std::string password;
    std::map<int, User*> users;
    struct sockaddr_in server_addr;
    int server_sock;
    int kqueue_fd;
    std::vector<struct kevent> changed;
    std::map<string, Channel*> channels;
    struct kevent event_list[10];
    std::queue<int> readFds;
    Translator *translator;
    std::map<std::string, ICommand*> commands;

private:
    int errorFlagLogic(struct kevent* currEvent);
    int readFlagLogic(struct kevent* currEvent);
    int writeFlagLogic(struct kevent* currEvent);
    int processCommand();
    void handleWriteFlag(int& writeFlag);
    void initCommands();

public:
    Server(int port, std::string password);
    ~Server();
    int getStatus() const;
    int makeServerSock();
    void addEvents(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
    void run();
    int connectClient();
    int checkEvent(int newEvent);
    User *getUser(int n);
    User *getUser(std::string& nick);
    std::map<int, User*>& getUsers();
    const std::string& getPassword() const;
    void addChannel(string s);
    std::map<string, Channel*> &getChannels();
    Channel *getChannel(string s);
    bool chkChannel(string s);
    void removeUser(int fd);
    bool searchUser(std::string& nickName);
};

vector<string> split(string input, char delimiter);
#endif
