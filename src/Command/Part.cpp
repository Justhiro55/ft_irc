#include "Command.hpp"

Part::Part() {}

Part::~Part() {}

void Part::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return sendToExecuter(ERR_NOTREGISTERED(executer->getNickname()) + "\r\n");	

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size < 1)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "PART") + "\r\n");
	if (message.trailing && params_size == 1)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "PART") + "\r\n");
	
	std::stringstream ss(params.front());
	std::string target;
	std::vector<std::string> targets;
	while (std::getline(ss, target,',')) {
		targets.push_back(target);
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
				sendToExecuter(ERR_NOTONCHANNEL(executer->getNickname(), target) + "\r\n");
				continue;
			}
			channel->unsetMember(executer);
			if (params_size == 1)
				sendToClients(channel->getClients(), MSG_PART_DEFAULT(executer->getNickname(), executer->getUsername(), executer->getHost(),
					target));
			else
				sendToClients(channel->getClients(), MSG_PART_REASON(executer->getNickname(), executer->getUsername(), executer->getHost(),
					target, params[1]));
		}
		else
			sendToExecuter(ERR_NOSUCHCHANNEL(executer->getNickname(), target) + "\r\n");
	}
}
