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
#include "utils.hpp"
#include "../srcs/Channel.hpp"
#include "../srcs/User.hpp"

using namespace std;

class Client
{
    public:
        int fd;
        string name;
    // public:
        Client(){
            fd = -1;
        }
        Client(const int f) {
            this->fd = f;
        }
        // Client(fd, name);
        ~Client() {

        }
};

int show_error(string s);

void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
                    uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
int init(int port);
int connect_client();
string receive(int fd);
int wait_event();
string get_cmd(string s);
void request(int fd, string cmd, string s);
int event_chk();
void disconnect(int fd);


extern struct sockaddr_in server_addr;
extern int server_sock;
extern int kqueue_fd;
extern std::vector<struct kevent> change_list; // kevent vector for changelist
extern struct kevent event_list[10];
extern int new_event;



#endif