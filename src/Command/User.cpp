#include "../includes/Command.hpp"

User::User() : AbstractCommand(){}

User::~User() {}

void User::executeCmd() {

	std::string nick = this->executer->getNickname().empty() ? "*" : this->executer->getNickname();

	if (!this->executer->getAuth()) {
		this->sendToExecuter(ERR_NOTREGISTERED(nick) + "\r\n");
		return ;
	}

	if (this->executer->getRegister()) {
		this->sendToExecuter(ERR_ALREADYREGISTRED(nick) + "\r\n");
		return ;
	}

	if (message.params.size() != 4) {
		this->sendToExecuter(ERR_NEEDMOREPARAMS(nick, "USER") + "\r\n");
		this->executer->setDisconnected(true);
		return ;
	}

	// Validate parameters are not empty
	if (message.params[0].empty() || message.params[1].empty() || 
		message.params[2].empty() || message.params[3].empty()) {
		this->sendToExecuter(ERR_NEEDMOREPARAMS(nick, "USER") + "\r\n");
		this->executer->setDisconnected(true);
		return ;
	}

	this->executer->setUsername(message.params[0]);
	this->executer->setHost(message.params[1]);
	this->executer->setServer(message.params[2]);
	this->executer->setRealname(message.params[3]);

	if (this->executer->checkAndCompleteRegistration()) {
		this->sendWelcomeMessages();
	}
}