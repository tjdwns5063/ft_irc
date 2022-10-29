#include "Server.hpp"

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "this program must exist two argument\n";
        return 1;
    }
    Server server = Server(atoi(argv[1]), argv[2]);
    if (server.getStatus() < 0)
        return 1;
    server.run();
}