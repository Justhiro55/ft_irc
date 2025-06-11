#include "../includes/Command.hpp"

User::User() : AbstractCommand(){}

User::~User() {}

void User::executeCmd() {

	if (this->executer->getAuth())
		return ; // 484  ERR_RESTRICTED ":Your connection is restricted!"

	if (!this->executer->getRegister())
		return ; // 462  ERR_ALREADYREGISTRED ":Unauthorized command (already registered)"

	if (message.params.size() != 4)
		return ;

	this->executer->setUsername(message.params[0]);
	this->executer->setHost(message.params[1]);
	this->executer->setServer(message.params[2]);
	this->executer->setRealname(message.params[3]);

	this->executer->setRegister(true);
}