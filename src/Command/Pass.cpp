#include "../includes/Command.hpp"

Pass::Pass() : AbstractCommand(){}

Pass::~Pass() {}

void Pass::executeCmd() {

	std::string param;

	if (this->executer->getAuth())
		return ; // 462  ERR_ALREADYREGISTRED ":Unauthorized command (already registered)"

	if (message.params.size() < 1)
		return ; // 461 ERR_NEEDMOREPARAMS "Pass :Not enough parameters"
	
	param = *(message.params.begin());
	if (serverData->verifyPassword(param))
		executer->setAuth(true);
	else
		return ; //464 ERR_PASSWDMISMATCH
}
