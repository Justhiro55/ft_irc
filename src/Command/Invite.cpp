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
		sendToExecuter(ERR_NOSUCHCHANNEL(executer->getNickname(), channel_name) + "\r\n");
		return;
	}

	if (!channel->getMemberByNick(executer->getNickname())) {
		sendToExecuter(ERR_NOTONCHANNEL(executer->getNickname(), channel_name) + "\r\n");
		return;
	}

	if (channel->hasMode(MODE_INVITE)) {
		if (!channel->isOperator(executer)) {
			sendToExecuter(ERR_CHANOPRIVSNEEDED(executer->getNickname(), channel_name) + "\r\n");
			return;
		}
	}

	Client* target_client = serverData->getClientByNickname(target_nick);
	if (!target_client) {
		sendToExecuter(ERR_NOSUCHNICK(target_nick) + "\r\n");
		return;
	}

	if (channel->getMemberByNick(target_nick)) {
		sendToExecuter(ERR_USERONCHANNEL(executer->getNickname(), target_nick, channel_name) + "\r\n");
		return;
	}

	if (channel->isInvited(target_nick)) {
		channel->removeInvite(target_nick);
	} else {
		channel->addInvite_list(target_nick);
	}

	sendToExecuter(RPL_INVITING(executer->getNickname(), channel_name, target_nick) + "\r\n");

	std::string invite_msg = ":" + executer->getNickname() + "!" + executer->getUsername() + "@" + executer->getIp() + " INVITE " + target_nick + " " + channel_name + "\r\n";
	target_client->pushToSendQueue(invite_msg);
	serverData->enablePollOut(target_client->getClientFd());
}
