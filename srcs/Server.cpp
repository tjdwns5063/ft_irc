#include "Server.hpp"
#include "Join.hpp"
#include "CUser.hpp"
#include "Kick.hpp"
#include "Kill.hpp"
#include "Nick.hpp"
#include "Oper.hpp"
#include "Part.hpp"
#include "Pass.hpp"
#include "Pong.hpp"
#include "Privmsg.hpp"
#include "Quit.hpp"
#include "Unknown.hpp"

void Server::initCommands() {
    commands["USER"] = new CUser(translator, ICommand::USER);
    commands["JOIN"] = new Join(translator, ICommand::JOIN);
    commands["KICK"] = new Kick(translator, ICommand::KICK);
    commands["KILL"] = new Kill(translator, ICommand::KILL);
    commands["kill"] = new Kill(translator, ICommand::KILL);
    commands["NICK"] = new Nick(translator, ICommand::NICK);
    commands["OPER"] = new Oper(translator, ICommand::OPER);
    commands["PART"] = new Part(translator, ICommand::PART);
    commands["PASS"] = new Pass(translator, ICommand::PASS);
    commands["PING"] = new Pong(translator, ICommand::PONG);
    commands["PRIVMSG"] = new Privmsg(translator, ICommand::PRIVMSG);
    commands["QUIT"] = new Quit(translator, ICommand::QUIT);
    commands["UNKNOWN"] = new Unknown(translator, ICommand::UNKNOWN);
}

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
    translator = new Translator();
    initCommands();
    addEvents(server_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
}

Server::~Server() {
    delete translator;
}

void Server::run() {
    while (1) {
        // std::system("leaks server");
        static timespec ts;
        ts.tv_sec = 10;
        ts.tv_nsec = 0;
        int newEvent = kevent(kqueue_fd, &changed[0], changed.size(), event_list, 10, &ts);
        changed.clear();
        checkEvent(newEvent);
    }
}

 static bool checkNewLineInBuf(Server& server, queue<int>& readFds) {
    if (server.getUsers().size() == 0)
        return false;
    char* buf = server.getUser(readFds.front())->getBuf();

    if (strchr(buf, '\n')) {
        return true ;
    }
    return false ;
}

int Server::checkEvent(int newEvent) {
    struct kevent* currEvent;

    for (int i = 0; i < newEvent; ++i) {
        currEvent = &event_list[i];
        if (currEvent->flags & EV_ERROR) {
            errorFlagLogic(currEvent);
            continue ;
        }
        if (currEvent->flags & EV_EOF) {
            std::vector<Channel*>& channels = getUser(currEvent->ident)->getChannels();
            for (unsigned long i = 0; i < channels.size(); ++i) {
                channels[i]->removeUser(*getUser(currEvent->ident));
            }
            removeUser(currEvent->ident);
            close(currEvent->ident);
            continue ;
        }
        if (currEvent->filter == EVFILT_READ) {
            readFlagLogic(currEvent);
        }
        if (currEvent->filter == EVFILT_WRITE) {
            writeFlagLogic(currEvent);
        }
    }
    while (!readFds.empty()) {
        processCommand();
    }
    return 0;
}

void Server::addEvents(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata) {
    struct kevent temp;

    EV_SET(&temp, ident, filter, flags, fflags, data, udata);
    changed.push_back(temp);
}

int Server::errorFlagLogic(struct kevent* currEvent) {
    status = -1;
    if ((int) currEvent->ident == server_sock) {
        std::cerr << "server socket error\n";
        return -1;
    }
    std::cerr << "client socket error\n";
    return -1;
}

int Server::readFlagLogic(struct kevent* currEvent) {
    int client_sock;

    if ((int) currEvent->ident == server_sock) { // server_socket에서 event가 발생 했을 때
        client_sock = connectClient();
        if (client_sock < 0) {
            status = -1;
            return -1;
        }
        users[client_sock] = new User(client_sock);
    } else { //client_socket에서 event가 발생했을 때
        char* buf = users[currEvent->ident]->getBuf();
        int len = recv(currEvent->ident, buf + strlen(buf), BUF_SIZE, 0);

        if (len < 0) {
            std::cerr << "receive error\n";
            close(currEvent->ident);
            removeUser(currEvent->ident);
            status = -1;
            return -1;
        }
        std::cout << "buf: " << buf << std::endl;
        readFds.push(currEvent->ident);
    }
    return 0;
}

int Server::writeFlagLogic(struct kevent* currEvent) {
    int len = strlen(users[currEvent->ident]->getBuf());
    char* buf = users[currEvent->ident]->getBuf();

    if (len > 0) {
        if (write(currEvent->ident, buf, len) < 0)
        {
            std::cerr << "write error\n";
            status = -1;
            return -1;
        }
        std::cout << "\twrited: " << buf << std::endl;
    }
    memset(buf, 0, BUF_SIZE);
    if ((getUser(currEvent->ident)->getFlag(KILLED))) {
        std::vector<Channel*> channels = getUser(currEvent->ident)->getChannels();
        for (unsigned long i = 0; i < channels.size(); ++i) {
            channels[i]->removeUser(*getUser(currEvent->ident));
        }
        removeUser(currEvent->ident);
        close(currEvent->ident);
    } else {
        addEvents(currEvent->ident, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    }
    return 1;
}

int Server::processCommand() {
    if (!checkNewLineInBuf(*this, readFds)) {
        readFds.pop();
        return 0;
    }
    std::vector<std::string> s = split(string(this->getUser(readFds.front())->getBuf()), '\n');
    for (int j = 0; j < (int)s.size(); j++)
    {
        std::vector<std::string> cmd = split(s[j], ' ');
        if (cmd[0] == "MODE" || cmd[0] == "PONG") {
            memset(getUser(readFds.front())->getBuf(), 0, BUF_SIZE);
            continue ;
        }
        if (!commands[cmd[0]]) {
            commands["UNKNOWN"]->execute(*this, cmd, readFds.front());
        }
        else
            commands[cmd[0]]->execute(*this, cmd, readFds.front());
    }
    s.clear();
    readFds.pop();
    return 0;
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

int Server::connectClient()
{
    int client_socket;

    if ((client_socket = accept(server_sock, NULL, NULL)) == -1) {
        std::cerr << "accept error\n";
        return -1;
    }
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    addEvents(client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    return client_socket;
}

User* Server::getUser(int n)
{
    return users[n];
}

User* Server::getUser(std::string& nick) {
    map<int, User*>::iterator it;
    for (it = users.begin(); it != users.end(); ++it) {
        if (it->second->getNickName() == nick)
            return it->second;
    }
    return it->second;
}

void Server::removeUser(int fd)
{
    if (users[fd]) {
        delete users[fd];
        users.erase(fd);
    }
}

void Server::addChannel(string s)
{
    channels[s] = new Channel(s);
}

std::map<string, Channel*> &Server::getChannels()
{
    return channels;
}

Channel* Server::getChannel(string s)
{
    if (channels.find(s) == channels.end())
        addChannel(s);
    return channels[s];
}

std::map<int, User*>& Server::getUsers() {
    return (this->users);
}

bool Server::chkChannel(string s)
{
    if (channels.find(s) == channels.end())
        return false;
    return true;
}

const std::string& Server::getPassword() const {
    return (this->password);
}

bool Server::searchUser(std::string& nickName) {
    std::map<int, User*>::iterator begin = users.begin();
    for (; begin != users.end(); ++begin) {
        if (begin->second->getNickName() == nickName) {
            return true;
        }
    }
    return false;
}
