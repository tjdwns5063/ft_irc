#include "../incs/Server.hpp"

Server::Server(int port, std::string password): port(port), password(password) {
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
        if (currEvent->filter == EVFILT_READ) {
            readFlagLogic(currEvent, writeFlag);
        }

        if (currEvent->filter == EVFILT_WRITE) {
            writeFlagLogic(currEvent, writeFlag);
        }
    }
    while (!readFds.empty()) {
        // 파싱하고 명령어에 따라 해당 클라이언트만 등록하거나 모두 등록하거나

        std::vector<std::string> s = split(string(this->getUser(readFds.front()).getBuf()), '\n');
        for (int j = 0 ; j < s.size(); j++)
        {
            std::cout << "s" << j << ": " << s[j] << std::endl;
        }
        for (int j = 0; j < s.size(); j++)
        {
            if (request(*this, readFds.front(), s[j]))
                return -1;
        }
        memset(users[readFds.front()].getBuf(), 0, sizeof(char) * 1024);
        readFds.pop();
    }
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

void Server::addEvents(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata) {
    struct kevent temp;

    EV_SET(&temp, ident, filter, flags, fflags, data, udata);
    changed.push_back(temp);
}

int Server::errorFlagLogic(struct kevent* currEvent) {
    status = -1;
    if (currEvent->ident == server_sock) {
        std::cerr << "server socket error\n";
        return -1;
    }
    std::cerr << "client socket error\n";
    return -1;
}

int Server::readFlagLogic(struct kevent* currEvent, int& writeFlag) {
    if (currEvent->ident == server_sock) { // server_socket에서 event가 발생 했을 때
        if (connectClient() < 0) {
            status = -1;
            return -1;
        }
    } else { //client_socket에서 event가 발생했을 때
        if (recv(currEvent->ident, users[currEvent->ident].getBuf(), sizeof(char) * 100, 0) <= 0) {
            std::cerr << "receive error\n";
            close(currEvent->ident);
            users.erase(currEvent->ident);
            status = -1;
            return -1;
        }
        std::cout << users[currEvent->ident].getBuf() <<std::endl;
        readFds.push(currEvent->ident);
        // addEvents(currEvent->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
    return 0;
}

int Server::writeFlagLogic(struct kevent* currEvent, int& writeFlag) {
    std::cout << "curr_event: write\n";
    // if (send(currEvent->ident, users[currEvent->ident].getBuf(), strlen(users[currEvent->ident].getBuf()) + 1, 0) == -1) {
    //     std::cerr << "write error\n";
    //     status = -1;
    //     return -1;
    // }
    memset(users[currEvent->ident].getBuf(), 0, sizeof(char) * 1024);
    addEvents(currEvent->ident, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    return 1;
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

void Server::addChannel(string s)
{
    // channels.insert(pair<std::string, Channel>(s, Channel()));
    channels[s] = Channel();
    std::cout << "make channel" << std::endl;
}

std::map<string, Channel> &Server::getChannels()
{
    return channels;
}

Channel &Server::getChannel(string s)
{
    if (channels.find(s) == channels.end())
        addChannel(s);
    return channels.find(s)->second;
}

std::map<int, User>& Server::getUsers() {
    return (this->users);
}

const std::string& Server::getPassword() const {
    return (this->password);
}
