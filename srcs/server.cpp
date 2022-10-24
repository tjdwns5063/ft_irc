#include "../incs/server.hpp"


struct sockaddr_in server_addr;
int server_sock;
int kqueue_fd;
std::vector<struct kevent> change_list; // kevent vector for changelist
struct kevent event_list[10];
int new_event;


string get_cmd(string s)
{
    int cmd_len = -1;
    string ret = "";
    while (s[++cmd_len] != ' ')
        ;
    int i = -1;
    while (++i < cmd_len)
        ret += s[i];
    return ret;
}

void request(int fd, string cmd, string s)
{
    if (cmd == "CAP")
        write(fd, ":aaa 001 aaa\n", strlen(":aaa 001 aaa\n"));
    else if (cmd == "JOIN") // channel join
        ;
    else if (cmd == "PRIVMSG") // send msg
    {

    }
    else if (cmd == "LEAVE" || cmd == "PART") // leave channel
    {

    }
    else if (cmd == "QUIT")
    {
        disconnect(fd);
    }
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