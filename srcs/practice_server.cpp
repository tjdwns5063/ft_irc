#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/event.h>
#include <vector>
#include <map>

void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

int main() {
    struct sockaddr_in server_addr;
    char buf[1024] = {0};
    int client_sock;
    int server_sock;
    int kqueue_fd;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4242);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    server_sock = socket(PF_INET, SOCK_STREAM, 0);


    if (server_sock == -1) {
        std::cout << "socket creation error\n";
        return 1;
    }

    if (bind(server_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
            std::cout << "bind error\n";
            return 1;
    }
    
    if(listen(server_sock, 5) == -1){
      printf( "listen() 실행 실패\n");
      exit( 1);
    }

    int log = open("log", O_CREAT | O_RDWR | O_TRUNC, 0777);
    if (log < 0) {
        std::cout << "log open failed\n";
    }
    
    kqueue_fd = kqueue();
    if (kqueue_fd == -1) {
        std::cout << "kqueue error\n";
    }

    std::map<int, std::string> clients; // map for client socket:data
    std::vector<struct kevent> change_list; // kevent vector for changelist
    struct kevent event_list[10];

    struct kevent temp;
    EV_SET(&temp, server_sock, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0); // a: kevent struct, b: ident, c: filter, d: flag, e: fflag, e:data,  f:udata
    change_list.push_back(temp);
    
    int new_event;
    struct kevent* curr_event;
    while (1) {
        new_event = kevent(kqueue_fd, &change_list[0], change_list.size(), event_list, 10, NULL);
        if (new_event == -1) {
            std::cout << "kevent error\n";
            return 1;
        }
        change_list.clear();
        std::cout << "new_event: " << new_event << '\n';   
        for (int i = 0; i < new_event; ++i) {
            curr_event = &event_list[i];
            if (curr_event->flags & EV_ERROR) {
                if (curr_event->ident == server_sock) {
                    std::cout << "server seocket error\n";
                }
                std::cout << "client socket error\n";
                return 1;
            }
            else if (curr_event->filter == EVFILT_READ) {
                std::cout << "curr_event: read\n";
                if (curr_event->ident == server_sock) { // server_socket에서 event가 발생 했을 때
                    /* accept new client */
                    // int client_socket;
                    int client_socket;
                    // socklen_t client_addr_size = sizeof(client_addr);
                    if ((client_socket = accept(server_sock, NULL, NULL)) == -1) {
                        std::cout << "accept error\n";
                        return 1;
                    }
                    std::cout << "accept new client: " << client_socket << std::endl;
                    fcntl(client_socket, F_SETFL, O_NONBLOCK);
                    /* add event for client socket - add read && write event */
                    change_events(change_list, client_socket, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    // change_events(change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                
                    // clients[client_socket] = "";
                }
                else { //client_socket에서 event가 발생했을 때
                    int n = read(curr_event->ident, buf, sizeof(buf));
                    if (n <= 0)
                        close(curr_event->ident);
                    write(log, buf, strlen(buf));
                    memset(buf, 0, 1024);
                }
            }

            // else if (curr_event->filter == EVFILT_WRITE) {
            //     std::cout << "curr_event: write\n";
            //     // read(client_sock, buf, 1024);
            //     // write(log, buf, 1024);
            //     // memset(buf, 0, 1024);
            //     // close(client_sock);
            // }
        }



        // 
        // client_sock = accept(server_sock, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_size);
        // if (client_sock == -1) {
        //     std::cout << "accept error\n";
        //     return 1;
        // }        
    }
}