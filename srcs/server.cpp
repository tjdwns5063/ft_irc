#include "../incs/server.hpp"

void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
                    uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

int show_error(string s)
{
    std::cerr << s;
    return 1;
}

int init(int port)
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)
        return show_error("socket creation error\n");
    if (bind(server_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
        return show_error("bind error\n");
    if(listen(server_sock, 5) == -1)
        return show_error("listen error\n");
    kqueue_fd = kqueue();
    if (kqueue_fd == -1)
        return show_error("queue error\n");
    change_events(change_list, server_sock, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0); // a: kevent struct, b: ident, c: filter, d: flag, e: fflag, e:data,  f:udata
        
    return 0;
}

int connect_client()
{

    int client_socket;
    // socklen_t client_addr_size = sizeof(client_addr);
    if ((client_socket = accept(server_sock, NULL, NULL)) == -1) 
        return show_error("accept error\n");
    std::cout << "accept new client: " << client_socket << std::endl;
    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    change_events(change_list, client_socket, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
    return 0;
}

char *receive(int fd)
{
    char *buf = (char *)malloc(4096);

    if (read(fd, buf, sizeof(buf)) <= 0)
    {
        close(fd);
        return NULL;
    }
    return buf;
}

int wait_event()
{
    new_event = kevent(kqueue_fd, &change_list[0], change_list.size(), event_list, 10, NULL);
    if (new_event == -1)
        return 1;
    change_list.clear();
    std::cout << "new_event: " << new_event << '\n';
    return 0;
}

int event_chk()
{
    char *s;
    struct kevent* curr_event;

    for (int i = 0; i < new_event; ++i) {
        curr_event = &event_list[i];
        if (curr_event->flags & EV_ERROR) {
            if (curr_event->ident == server_sock)
                return show_error("server socket error\n");
            else
                return show_error("client socket error\n");
        }
        else if (curr_event->filter == EVFILT_READ) {
            std::cout << "curr_event: read\n";
            if (curr_event->ident == server_sock)     // server_socket에서 event가 발생 했을 때
                connect_client();
            else { //client_socket에서 event가 발생했을 때
                if (!(s = receive(curr_event->ident)))
                    show_error("receive error\n");
                else
                    cout << s << endl;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    // int port = atoi(argv[1]);
    init(4242);
    map<int, Client> clients;
    char *s;

    while (1) {        
        if (wait_event())
            return show_error("kevent error\n");
        if (event_chk())
            return 1;
    }
    return 0;
}