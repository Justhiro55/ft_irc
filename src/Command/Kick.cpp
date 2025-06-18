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
		sendToExecuter(ERR_BADCHANMASK(executer->getNickname(), channel_name) + "\r\n");
		return;
	}

	Channel* channel = serverData->getChannelByName(channel_name);
	if (!channel) {
		sendToExecuter(ERR_NOSUCHCHANNEL(executer->getNickname(), channel_name) + "\r\n");
		return;
	}

	if (!channel->getMemberByNick(executer->getNickname())) {
		sendToExecuter(ERR_NOTONCHANNEL(executer->getNickname(), channel_name) + "\r\n");
		return;
	}

	Client* target_client = serverData->getClientByNickname(target_nick);
	if (!target_client) {
		sendToExecuter(ERR_NOSUCHNICK(target_nick) + "\r\n");
		return;
	}

	if (!channel->getMemberByNick(target_nick)) {
		sendToExecuter(ERR_USERNOTINCHANNEL(target_nick, channel_name) + "\r\n");
		return;
	}

	if (!channel->isOperator(executer)) {
		sendToExecuter(ERR_CHANOPRIVSNEEDED(executer->getNickname(), channel_name) + "\r\n");
		return;
	}

	channel->unsetMember(target_client);

	std::string kick_msg = ":" + executer->getNickname() + "!" + executer->getUsername() + "@" + executer->getIp() +
	                       " KICK " + channel_name + " " + target_nick + " :" + reason + "\r\n";

	sendToExecuter(kick_msg);
	target_client->pushToSendQueue(kick_msg);
	// TODO: Send to channel members
}
