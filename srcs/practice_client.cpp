#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {
    struct sockaddr_in server_addr;
    int client_sock = socket(PF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(4242);

    if (connect(client_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cout << "fail\n";
    }
    while (1) {
        std::string str;
        std::cin >> str;
        if (str == "exit")
            break ;
        write(client_sock, str.append("\n").c_str(), str.size());
    }
    // std::string data;
    // data = "push\n";
    // write(client_sock, data.c_str(), data.size());
    close(client_sock);
    return (0);
}