#include "Command.hpp"

Oper::Oper() :AbstractCommand() {}

Oper::~Oper() {}

void Oper::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return sendToExecuter(ERR_NOTREGISTERED(executer->getNickname()) + "\r\n");	

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size != 2)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "Privmsg") + "\r\n");
	
	if (params[0] != OPER_NAME || params[1] != OPER_PASSWORD)
		return sendToExecuter(ERR_PASSWDMISMATCH(executer->getNickname()));
	if (executer->hasMode(USER_MODE_OPERATOR))
		return sendToExecuter(RPL_YOUREOPER + "\r\n");
	executer->setMode(USER_MODE_OPERATOR);
	sendToExecuter(RPL_YOUREOPER + "\r\n");
	sendToExecuter(PRL_UMODECHANGE(executer->getNickname(), "+o") + "\r\n");
}
