#include "Command.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

void Privmsg::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return sendToExecuter(ERR_NOTREGISTERED(executer->getNickname()) + "\r\n");	

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size < 1)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "Privmsg") + "\r\n");
	if (params_size == 1 && message.trailing)
		return sendToExecuter(ERR_NORECIPIENT(message.command) + "\r\n");
	else if (params_size == 1 && !message.trailing)
		return sendToExecuter(ERR_NOTEXTTOSEND + "\r\n");
	
	std::string text = params[1];
	std::stringstream ss(params.front());
	std::string target;
	while (std::getline(ss, target,',')) {
		if ( params.front()[0] == '#' || params.front()[0] == '&' ) {
			Channel *channel = serverData->getChannelByName(target);
			if (channel == NULL)
				return sendToExecuter(ERR_NOSUCHCHANNEL(target + "\r\n"));
			channel->sendToMembers(MSG_PRIVMSG(executer->getNickname(), executer->getUsername(), executer->getHost(),
				target, text) + "\r\n", executer->getNickname());
		} else {
			Client *client = serverData->getClientByNickname(target);
			if (client == NULL)
				return sendToExecuter(ERR_NOSUCHNICK(target + "\r\n"));
			sendToClient(client, MSG_PRIVMSG(executer->getNickname(), executer->getUsername(),
				executer->getHost(), target, text) + "\r\n");
		}
	}
}
