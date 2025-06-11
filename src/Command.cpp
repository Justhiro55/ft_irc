#include "Message.hpp"
#include "Client.hpp"
#include "Command.hpp"

bool Nick::is_valid_nickname(const std::string& nick) {
    if (nick.empty() || nick.length() > 9)
        return false;

    char first = nick[0];
    if (!std::isalpha(static_cast<unsigned char>(first)) &&
        first != '[' && first != ']' && first != '\\' &&
        first != '`' && first != '_' && first != '^' &&
        first != '{' && first != '}')
        return false;

    for (std::string::size_type i = 1; i < nick.length(); ++i) {
        char c = nick[i];
        if (!std::isalnum(static_cast<unsigned char>(c)) &&
            c != '[' && c != ']' && c != '\\' &&
            c != '`' && c != '_' && c != '^' &&
            c != '{' && c != '}' && c != '-')
            return false;
    }

    return true;
}

void Nick::executeCmd() {

	Client *target;
	std::string param;

	if (this->executer->getAuth())
		return ; // 484  ERR_RESTRICTED ":Your connection is restricted!"

	if (message.params.size() < 1)
		return ; // 431 ERR_NONICKNAMEGIVEN ":No nickname given"

	param = *(message.params.begin());

	if (!is_valid_nickname(param))
		return ; // "432 ERR_ERRONEUSNICKNAME <nick> :Erroneous nickname"
	
	target = this->serverData->getClientByNickname(param);
	if (target != NULL)
	 	return ; //  433 ERR_NICKNAMEINUSE "<nick> :Nickname is already in use"

	this->executer->setNickname(param);
}


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

