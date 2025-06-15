#include "../includes/Command.hpp"

Kick::Kick() : AbstractCommand() {}

Kick::~Kick() {}

void Kick::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister()) {
		sendToExecuter(ERR_RESTRICTED(executer->getNickname()) + "\r\n");
		return;
	}

	if (message.params.size() < 2) {
		sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "KICK") + "\r\n");
		return;
	}

	std::string channel_name = message.params[0];
	std::string target_nick = message.params[1];
	std::string reason;
	if (message.params.size() >= 3) {
		reason = message.params[2];
	} else {
		reason = executer->getNickname();
	}

	if (channel_name.empty() || channel_name[0] != '#') {
		sendToExecuter("476 " + executer->getNickname() + " " + channel_name + " :Bad Channel Mask\r\n");
		return;
	}

	Channel* channel = serverData->getChannelByName(channel_name);
	if (!channel) {
		sendToExecuter("403 " + executer->getNickname() + " " + channel_name + " :No such channel\r\n");
		return;
	}

	if (!channel->getMemberByNick(executer->getNickname())) {
		sendToExecuter("442 " + executer->getNickname() + " " + channel_name + " :You're not on that channel\r\n");
		return;
	}

	Client* target_client = serverData->getClientByNickname(target_nick);
	if (!target_client) {
		sendToExecuter("401 " + executer->getNickname() + " " + target_nick + " :No such nick/channel\r\n");
		return;
	}

	if (!channel->getMemberByNick(target_nick)) {
		sendToExecuter("441 " + executer->getNickname() + " " + target_nick + " " + channel_name + " :They aren't on that channel\r\n");
		return;
	}

	if (!channel->isOperator(executer)) {
		sendToExecuter("482 " + executer->getNickname() + " " + channel_name + " :You're not channel operator\r\n");
		return;
	}

	channel->unsetMember(target_client);

	std::string kick_msg = ":" + executer->getNickname() + "!" + executer->getUsername() + "@" + executer->getIp() +
	                       " KICK " + channel_name + " " + target_nick + " :" + reason + "\r\n";

	sendToExecuter(kick_msg);
	target_client->pushToSendQueue(kick_msg);
	// TODO: Send to channel members
}
