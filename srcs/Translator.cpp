// #include "server.hpp"
#include "Translator.hpp"

Translator::Translator() {}
Translator::~Translator() {}


std::string Translator::translateUserMsg(std::string& msg) {

}

std::string Translator::translateNickMsg(std::string& msg) {
    // 1. 이미 존재하는 닉인지 확인. 이미 있으면, ERR_NICKNAMEINUSE 반환.
    // 2. 해당 닉이 사용 가능한지 확인.(사용 가능한 글자가 있는지 등) 불가능 하다면, ERR_ERRONEUSNICKNAME 반환
    // 3. 매개변수가 없이 들어온 명령인지 확인. 없다면, ERR_NONICKNAMEGIVEN 반환.
    // 4. 명령어에 하자가 없다면, 해당 클라이언트의 nick 변경. 후 ":(이전 닉네임) NICK (이후 닉네임)" 형식으로 메시지 반환.
}

std::string Translator::translate(std::string& msg) {
    // 1. 이미 해당 유저가 등록 되었으면 ERR_ALREADYREGISTERED 응답.
    // 2. 그렇지 않으면 유저 등록.
    // 3. 후에 클라이언트로 연결 메세지 전송.

    // int len = msg.find(' ', 0);
    // std::string substr = msg.substr(0, len);
    // users.find()
}

int request(Server &server, int fd, vector<std::string> cmd)
{
    int n = 0;
    if (cmd[0] == "CAP")
    {
        n = write(fd, ":aaa 001 aaa\n", strlen(":aaa 001 aaa\n"));
        if (n < 0)
            return 1;
    }
    else if (cmd[0] == "NICK")
    {
        server.getUser(fd).setNickName(cmd[1]);
        std::cout << "nickname : " << server.getUser(fd).getNickName() << std::endl;
    }
    else if (cmd[0] == "USER")
    {
        server.getUser(fd).setUserName(cmd[1]);
        std::cout << "username : " << server.getUser(fd).getUserName() << std::endl;
    }
    else if (cmd[0] == "JOIN") // channel join
        ;
    else if (cmd[0] == "PRIVMSG") // send msg
    {

    }
    else if (cmd[0] == "LEAVE" || cmd[0] == "PART") // leave channel
    {

    }
    else if (cmd[0] == "QUIT")
    {
        close(fd);
    }
    return 0;
}