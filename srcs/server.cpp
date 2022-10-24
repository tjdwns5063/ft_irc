#include "../incs/Server.hpp"

int Server::connectClient()
{
    int client_socket;

    if ((client_socket = accept(server_sock, NULL, NULL)) == -1) 
        std::cerr << "accept error\n";
    std::cout << "accept new client: " << client_socket << std::endl;
    users[client_socket] = User(client_socket);
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    addEvents(client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    addEvents(client_socket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);
    return 0;
}

void Server::addEvents(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata) {
    struct kevent temp;

    EV_SET(&temp, ident, filter, flags, fflags, data, udata);
    changed.push_back(temp);
}

int Server::checkEvent(int newEvent) {
    struct kevent* curr_event;
    int writeFlag = 0;

    for (int i = 0; i < newEvent; ++i) {
        curr_event = &event_list[i];
        if (curr_event->flags & EV_ERROR) {
            if (curr_event->ident == server_sock) {
                std::cerr << "server socket error\n";
                return 1;
            }
            else {
                std::cerr << "client socket error\n";
                return 1;
            }
        }
        if (curr_event->filter == EVFILT_READ) {
            std::cout << "curr_event: read\n";
            if (curr_event->ident == server_sock) {    // server_socket에서 event가 발생 했을 때
                connectClient();
            }
            else { //client_socket에서 event가 발생했을 때
                if (recv(curr_event->ident, buf, sizeof(buf), 0) == -1)
                    std::cerr << "receive error\n";
                if (curr_event->ident != server_sock) {
                    std::string temp = std::string(buf);
                    size_t len = temp.find('\n');
                    if (len != std::string::npos) {
                        writeFlag = 1;
                    }
                }
            }
        }

        if (curr_event->filter == EVFILT_WRITE) {
            std::cout << "curr_event: write\n";
            std::cout << "curr fd: " << curr_event->ident << '\n';
            // std::cout << "buf: " << buf;
            writeFlag = 2;
            send(curr_event->ident, std::string(buf).c_str(), strlen(buf), 0);
            // EV_SET(&changed, curr_event->ident, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
        }
    }
    if (writeFlag > 0) {
        int filter;
        if (writeFlag == 1)
            filter = EV_ENABLE;
        else
            filter = EV_DISABLE;
        for (std::map<int, User>::iterator it = users.begin(); it != users.end(); ++it) {
            addEvents(it->first, EVFILT_WRITE, filter, 0, 0, NULL);
        }
        if (writeFlag == 2)
            memset(buf, 0, sizeof(buf));
    }
    return 0;
}

Server::Server(int port, std::string password): port(port), password(password) {
    memset(buf, 0, sizeof(buf));
    server_sock = makeServerSock();
    status = 0;

    if (server_sock == -1) {
        status = -1;
    }
    if (bind(server_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        std::cerr << "bind error\n";
        status = -1;
    }

    if (listen(server_sock, 5) == -1) {
        std::cerr << "listen error\n";
        status = -1;
    }

    if ((kqueue_fd = kqueue()) == -1) {
        status = -1;
        std::cerr << "kqueue error\n";
    }
    addEvents(server_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
}

Server::~Server() {}

int Server::getStatus() const {
    return status;
}

int Server::makeServerSock() {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        std::cerr << "socket creation error\n";
        return -1;
    }
    return server_sock;
}

void Server::run() {
    while (1) {
        int newEvent = kevent(kqueue_fd, &changed[0], changed.size(), event_list, 10, NULL);
        changed.clear();
        std::cout << "newEvent: " << newEvent << '\n';
        checkEvent(newEvent);
    }
}
