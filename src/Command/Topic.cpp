#include "Command.hpp"

Topic::Topic() {}

Topic::~Topic() {}

void Topic::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return sendToExecuter(ERR_NOTREGISTERED(executer->getNickname()) + "\r\n");	

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size < 1)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "Privmsg") + "\r\n");
	if (params_size == 1 && message.trailing)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "Privmsg") + "\r\n");
	
	if ( params.front()[0] == '#' || params.front()[0] == '&' ) {
        Channel* channel;
		std::string channel_name = params.front();

		if ( channel_name.size() <= 1 )
            return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "MODE") + "\r\n");
			
        channel = serverData->getChannelByName(channel_name);
		if (channel == NULL)
			return sendToExecuter(ERR_NOSUCHCHANNEL(executer->getNickname(), channel_name) + "\r\n");

		if (!channel->isMember(executer->getNickname()))
			return sendToExecuter(ERR_NOTONCHANNEL(executer->getNickname(), channel_name) + "\r\n");

		if (params_size == 1) {
			std::string channel_topic = channel->getTopic();
			if (channel_topic.empty())
				return sendToExecuter(RPL_NOTOPIC(channel_name) + "\r\n");
			else
				return sendToExecuter(RPL_TOPIC(channel_name, channel_topic) + "\r\n");
		}

		if (channel->hasMode(MODE_TOPICLOCK) && channel->isOperatorMember(executer->getNickname()))
			return sendToExecuter(ERR_CHANOPRIVSNEEDED(executer->getNickname(), channel_name) + "\r\n");

		if (message.trailing && params[1].empty() && params_size == 2)
			channel->clearTopic();
		else
			channel->setTopic(params[1]);

		return channel->sendToMembers(MSG_TOPIC(executer->getNickname(), executer->getHost(), executer->getHost(),
			channel_name,  params[1]), "");
	}
	return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "MODE") + "\r\n");
}
