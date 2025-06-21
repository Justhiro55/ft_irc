#include "../includes/Command.hpp"

Ping::Ping() : AbstractCommand(){}

Ping::~Ping() {}

void Ping::executeCmd() {
	std::string nick = this->executer->getNickname().empty() ? "*" : this->executer->getNickname();

	if (message.params.empty() || message.params[0].empty()) {
		this->sendToExecuter(ERR_NEEDMOREPARAMS(nick, "PING") + "\r\n");
		return;
	}

	std::string pingParam = message.params[0];
	std::string pongReply = "PONG :" + pingParam + "\r\n";

	this->sendToExecuter(pongReply);
}
