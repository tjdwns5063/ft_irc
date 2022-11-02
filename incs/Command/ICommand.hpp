#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

#include <string>
#include <vector>

class Translator;
class Server;

// void cmd_nick(Server &server, int fd, std::vector<std::string> cmd);

// void cmd_user(Server &server, int fd, std::vector<std::string> cmd);
// void cmd_join(Server &server, int fd, std::vector<std::string> cmd);
// void cmd_privmsg(Server &server, int fd, std::vector<std::string> cmd);

// void cmd_part(Server &server, int fd, std::vector<std::string> cmd);
// void cmd_quit(Server &server, int fd);

// void cmd_kick(Server &server, int fd, std::vector<std::string>& cmd);
// void cmd_oper(Server &server, int fd, std::vector<std::string>& cmd);
// void cmd_pass(Server &server, int fd, std::vector<std::string>& cmd);

// void cmd_kill(Server &server, int fd, std::vector<std::string>& cmd);
// void cmd_unknown(Server& server, int fd, std::vector<std::string>& cmd);
// void cmd_ping(Server &server, int fd, std::vector<std::string>& cmd);

class ICommand {
public:
	enum CommandType {
		PASS,
		NICK,
		USER,
		JOIN,
		PRIVMSG,
		PART,
		QUIT,
		KICK,
		OPER,
		KILL,
		PONG,
		UNKNOWN
	};
protected:
	Translator* translator;
	CommandType type;
public:
	ICommand(Translator* translator, CommandType type);
	virtual ~ICommand();
	CommandType getType() const;
	// virtual int publishResultCode(Server& server, std::vector<std::string>& cmd, int fd) = 0;
	virtual void execute(Server& server, std::vector<std::string>& cmd, int fd) = 0;
};

#endif