#include "../includes/Command.hpp"

Ping::Ping() : AbstractCommand(){}

Ping::~Ping() {}

void Ping::executeCmd() {
      if (message.params.empty() || message.params[0].empty()) {
          return;
      }

	std::string pingParam = message.params[0];
	std::string pongReply = "PONG :" + pingParam + "\r\n";

	this->sendToExecuter(pongReply);
}
