#include "../includes/Message.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"

Nick::Nick() : AbstractCommand(){}

Nick::~Nick() {}

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
	std::string nick = this->executer->getNickname().empty() ? "*" : this->executer->getNickname();

	if (!this->executer->getAuth()) {
		this->sendToExecuter(ERR_RESTRICTED(nick) + "\r\n");
		return ;
	}

	if (message.params.size() < 1) {
		this->sendToExecuter(ERR_NONICKNAMEGIVEN(nick) + "\r\n");
		return ;
	}

	param = *(message.params.begin());

	if (!is_valid_nickname(param)) {
		this->sendToExecuter(ERR_ERRONEUSNICKNAME(nick, param) + "\r\n");
		return ;
	}
	
	target = this->serverData->getClientByNickname(param);
	if (target != NULL) {
		this->sendToExecuter(ERR_NICKNAMEINUSE(nick, param) + "\r\n");
		return ;
	}

	this->executer->setNickname(param);
	
	if (this->executer->isFullyRegistered()) {
		this->sendWelcomeMessages();
	}
}
