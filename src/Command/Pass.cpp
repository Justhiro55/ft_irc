#include "../includes/Command.hpp"

Pass::Pass() : AbstractCommand(){}

Pass::~Pass() {}

void Pass::executeCmd() {

	std::string param;
	std::string nick = this->executer->getNickname().empty() ? "*" : this->executer->getNickname();

	if (this->executer->getAuth()) {
		this->sendToExecuter(ERR_ALREADYREGISTRED(nick) + "\r\n");
		return ;
	}

	if (message.params.size() < 1) {
		this->sendToExecuter(ERR_NEEDMOREPARAMS(nick, "PASS") + "\r\n");
		return ;
	}
	
	param = *(message.params.begin());
	if (serverData->verifyPassword(param)) {
		executer->setAuth(true);
	} else {
		this->sendToExecuter(ERR_PASSWDMISMATCH(nick) + "\r\n");
		return ;
	}
}
