#include "../incs/Server.hpp"

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

void Server::run() {
    while (1) {
        int newEvent = kevent(kqueue_fd, &changed[0], changed.size(), event_list, 10, NULL);
        changed.clear();
        std::cout << "newEvent: " << newEvent << '\n';
        checkEvent(newEvent);
    }
}

int Server::checkEvent(int newEvent) {
    struct kevent* currEvent;
    int writeFlag = 0;
    for (int i = 0; i < newEvent; ++i) {
        currEvent = &event_list[i];
        if (currEvent->flags & EV_ERROR) {
            errorFlagLogic(currEvent);
        }
        if (currEvent->flags & EV_EOF) {
            errorFlagLogic(currEvent);
        }
        if (currEvent->filter == EVFILT_READ) {
            readFlagLogic(currEvent, writeFlag);
        }
        if (currEvent->filter == EVFILT_WRITE) {
            writeFlagLogic(currEvent, writeFlag);
        }
    }
    handleWriteFlag(writeFlag);
    return 0;
}

int Server::connectClient()
{
    int client_socket;

    if ((client_socket = accept(server_sock, NULL, NULL)) == -1) {
        std::cerr << "accept error\n";
        return 1;
    }
    std::cout << "accept new client: " << client_socket << std::endl;
    users[client_socket] = User(client_socket);
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    addEvents(client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    return 0;
}

int Server::errorFlagLogic(struct kevent* currEvent) {
    status = -1;
    if (currEvent->ident == server_sock) {
        std::cerr << "server socket error\n";
        return -1;
    }
    std::cerr << "client socket error\n";
    exit(1);
    close(currEvent->ident);
    return -1;
}

void Server::addEvents(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata) {
    struct kevent temp;

    EV_SET(&temp, ident, filter, flags, fflags, data, udata);
    changed.push_back(temp);
}

int Server::readFlagLogic(struct kevent* currEvent, int& writeFlag) {
    if (currEvent->ident == server_sock) {    // server_socket에서 event가 발생 했을 때
        if (connectClient() < 0) {
            status = -1;
            return -1;
        }
    } else { //client_socket에서 event가 발생했을 때
        if (recv(currEvent->ident, buf, sizeof(buf), 0) <= 0) {
            std::cerr << "receive error\n";
            close(currEvent->ident);
            users.erase(currEvent->ident);
            status = -1;
            return -1;
        }
        std::cout << "buf: " << buf << '\n';
        std::string temp = std::string(buf);
        size_t len = temp.find('\n');
        if (len != std::string::npos) {
            writeFlag = 1;
        }
    }
    return 0;
}

int Server::writeFlagLogic(struct kevent* currEvent, int& writeFlag) {
    std::cout << "curr_event: write\n";
    writeFlag = 2;
    std::vector<std::string> s = split(buf, '\n');
    std::cout << "size: "<< s.size() << std::endl;
    for (int j = 0; j < s.size(); j++)
    {
        if (request(*this, currEvent->ident, split(s[j], ' ')));
            return -1;
    }
    return 0;
}

void Server::handleWriteFlag(int& writeFlag) {
    int filter = 0;

    if (writeFlag <= 0)
        return ;
    for (std::map<int, User>::iterator it = users.begin(); it != users.end(); ++it) {
        if (writeFlag == 1)
            addEvents(it->first, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
        else
            addEvents(it->first, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    }
    if (writeFlag == 2)
        memset(buf, 0, sizeof(buf));
}

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

User& Server::getUser(int n)
{
    return users[n];

}