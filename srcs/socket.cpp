#include "../incs/server.hpp"


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

string receive(int fd)
{
    char buf[4096];
    int n;
    if ((n = read(fd, buf, 4096)) <= 0)
    {
        close(fd);
        return NULL;
    }
    buf[n] = '\0';
    string ret(buf);
    return ret;
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

void disconnect(int fd)
{
	close(fd);
    change_events(change_list, fd, EVFILT_READ | EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
}

int event_chk()
{
    string s;
    struct kevent* curr_event;

    for (int i = 0; i < new_event; ++i) {
        curr_event = &event_list[i];
        if (curr_event->flags & EV_ERROR) {
            if (curr_event->ident == server_sock)
                return show_error("server socket error\n");
            else
                return show_error("client socket error\n");
        }
        else if (curr_event->flags & EV_EOF)
        	disconnect(curr_event->ident);
        else if (curr_event->filter == EVFILT_READ) {
            std::cout << "curr_event: read\n";
            if (curr_event->ident == server_sock)     // server_socket에서 event가 발생 했을 때
                connect_client();
            else { //client_socket에서 event가 발생했을 때
                if ((s = receive(curr_event->ident)).empty())
                    show_error("receive error\n");
                else
                {
                    char *tok = strtok(const_cast<char *>(s.c_str()), "\n");
                    while (tok)
                    {
                        cout << tok << endl;
                        request(curr_event->ident, get_cmd(string(tok)), string(tok));
                        // cout << get_cmd(string(tok)) << endl;
                        tok = strtok(NULL, "\n");
                    }
                }
            }
        }
    }
    return 0;
}
