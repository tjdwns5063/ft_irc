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