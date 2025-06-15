#include "../includes/Command.hpp"

Invite::Invite() : AbstractCommand() {}

Invite::~Invite() {}

void Invite::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister()) {
		sendToExecuter(ERR_RESTRICTED(executer->getNickname()) + "\r\n");
		return;
	}

	if (message.params.size() != 2) {
		sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "INVITE") + "\r\n");
		return;
	}

	std::string target_nick = message.params[0];
	std::string channel_name = message.params[1];

	Channel* channel = serverData->getChannelByName(channel_name);
	if (!channel) {
		sendToExecuter("403 " + executer->getNickname() + " " + channel_name + " :No such channel\r\n");
		return;
	}

	if (!channel->getMemberByNick(executer->getNickname())) {
		sendToExecuter("442 " + executer->getNickname() + " " + channel_name + " :You're not on that channel\r\n");
		return;
	}

	if (channel->hasMode(MODE_INVITE)) {
	}

	Client* target_client = serverData->getClientByNickname(target_nick);
	if (!target_client) {
		sendToExecuter("401 " + executer->getNickname() + " " + target_nick + " :No such nick/channel\r\n");
		return;
	}

	if (channel->getMemberByNick(target_nick)) {
		sendToExecuter("443 " + executer->getNickname() + " " + target_nick + " " + channel_name + " :is already on channel\r\n");
		return;
	}

	channel->addInvite_list(target_nick);

	sendToExecuter("341 " + executer->getNickname() + " " + channel_name + " " + target_nick + "\r\n");

	std::string invite_msg = ":" + executer->getNickname() + "!" + executer->getUsername() + "@" + executer->getIp() + " INVITE " + target_nick + " " + channel_name + "\r\n";
	target_client->pushToSendQueue(invite_msg);
}
