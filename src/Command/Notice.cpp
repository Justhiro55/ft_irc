#include "Command.hpp"

Notice::Notice() {}

Notice::~Notice() {}

void Notice::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return sendToExecuter(ERR_NOTREGISTERED(executer->getNickname()) + "\r\n");	

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size < 1)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "Notice") + "\r\n");
	if (params_size == 1 && message.trailing)
		return sendToExecuter(ERR_NORECIPIENT(message.command) + "\r\n");
	else if (params_size == 1 && !message.trailing)
		return sendToExecuter(ERR_NOTEXTTOSEND(executer->getNickname()) + "\r\n");
	
	std::string text = params[1];
	std::stringstream ss(params.front());
	std::string target;
	std::vector<std::string> targets;
	while (std::getline(ss, target,',')) {
		targets.push_back(target);
	}

	if (targets.size() > 5) {
		std::vector<std::string>::iterator overTarget = targets.begin() + 5;
		sendToExecuter(ERR_TOOMANYTARGETS(executer->getNickname(), *overTarget) + "\r\n");
		targets.erase(targets.begin() + 5, targets.end());
	}

	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it) {
		std::string target = *it;
		if ( target[0] == '#' || target[0] == '&' ) {
			Channel *channel = serverData->getChannelByName(target);
			if (channel == NULL) {
				sendToExecuter(ERR_NOSUCHCHANNEL(executer->getNickname(), target) + "\r\n");
				continue;
			}
			if (!channel->isMember(executer->getNickname())) {
				sendToExecuter(ERR_CANNOTSENDTOCHAN(executer->getNickname(), channel->getName()) + "\r\n");
				continue;
			}

			std::vector<Client *> sendingClients = channel->getClients();
			std::vector<Client*>::iterator it = std::find(sendingClients.begin(), sendingClients.end(), executer);
			if (it != sendingClients.end())
				sendingClients.erase(it);
			sendToClients(sendingClients, MSG_NOTICE(executer->getNickname(), executer->getUsername(), executer->getHost(),
				target, text));
		} else {
			Client *client = serverData->getClientByNickname(target);
			if (client == NULL)
				return sendToExecuter(ERR_NOSUCHNICK(target) + "\r\n");
			sendToClient(client, MSG_NOTICE(executer->getNickname(), executer->getUsername(),
				executer->getHost(), target, text));
		}
	}
}

void Notice::removeDuplicates(std::vector<std::string>& vec) {
    std::sort(vec.begin(), vec.end());  // 昇順ソート
    std::vector<std::string>::iterator it = std::unique(vec.begin(), vec.end());
    vec.erase(it, vec.end());
}