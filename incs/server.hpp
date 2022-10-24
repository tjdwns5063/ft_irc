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

struct sockaddr_in server_addr;
int server_sock;
int kqueue_fd;
std::vector<struct kevent> change_list; // kevent vector for changelist
struct kevent event_list[10];
int new_event;


#endif