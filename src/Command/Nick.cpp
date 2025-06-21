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
		this->sendToExecuter(ERR_NOTREGISTERED(nick) + "\r\n");
		return ;
	}

	if (message.params.size() < 1) {
		this->sendToExecuter(ERR_NONICKNAMEGIVEN(nick) + "\r\n");
		if (!this->executer->getRegister()) {
			this->executer->setDisconnected(true);
		}
		return ;
	}

	param = *(message.params.begin());

	if (param.empty()) {
		this->sendToExecuter(ERR_NONICKNAMEGIVEN(nick) + "\r\n");
		if (!this->executer->getRegister()) {
			this->executer->setDisconnected(true);
		}
		return ;
	}

	if (!is_valid_nickname(param)) {
		this->sendToExecuter(ERR_ERRONEUSNICKNAME(nick, param) + "\r\n");
		if (!this->executer->getRegister()) {
			this->executer->setDisconnected(true);
		}
		return ;
	}

	target = this->serverData->getClientByNickname(param);
	if (target != NULL) {
		this->sendToExecuter(ERR_NICKNAMEINUSE(nick, param) + "\r\n");
		if (!this->executer->getRegister()) {
			this->executer->setDisconnected(true);
		}
		return ;
	}

	std::string old_nick = this->executer->getNickname();
	bool is_registered = this->executer->getRegister();

	this->executer->setNickname(param);

	if (is_registered && !old_nick.empty()) {
		std::string nick_msg = ":" + old_nick + "!" + this->executer->getUsername() + "@" + this->executer->getIp() +
		                      " NICK :" + param + "\r\n";

		this->sendToExecuter(nick_msg);

		std::vector<Channel*> user_channels = this->executer->getChannels();
		for (size_t i = 0; i < user_channels.size(); i++) {
			if (!this->serverData->isValidChannel(user_channels[i])) {
				continue;
			}
			std::vector<Client*> channel_members = user_channels[i]->getClients();
			for (size_t j = 0; j < channel_members.size(); j++) {
				if (channel_members[j]->getNickname() != old_nick && !channel_members[j]->isDisconnected()) {
					this->sendToClient(channel_members[j], nick_msg);
				}
			}
		}
	}

	if (this->executer->checkAndCompleteRegistration()) {
		this->sendWelcomeMessages();
	}
}
