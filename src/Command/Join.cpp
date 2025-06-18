#include "../includes/Command.hpp"
#include "map"

bool isValidChannelName(const std::string& channel_name);

Join::Join() :AbstractCommand() {}

Join::~Join() {}

void Join::executeCmd() {
	size_t params_size = message.params.size();
	std::vector<std::pair<std::string, std::string> > param_channels;
	
	if (!this->executer->getAuth() || !this->executer->getRegister()) {
		sendToExecuter(ERR_RESTRICTED(executer->getNickname()) + "\r\n");
		return;
	}
	
	if (params_size < 1 || params_size > 2) {
		sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "JOIN") + "\r\n");
		return;
	}

	std::stringstream ss(message.params[0]);
	std::string channel_name;

	while (std::getline(ss, channel_name, ',')) {
		param_channels.push_back(std::make_pair(channel_name, ""));
	}

	if (params_size == 2) {
		std::stringstream ss_key(message.params[1]);
		std::string key;
		std::vector<std::pair<std::string, std::string> >::iterator it = param_channels.begin();

		while (std::getline(ss_key, key, ',')) {
			if (it == param_channels.end())
				break;
			it->second = key;
			++it;
		}
	}

	for(std::vector<std::pair<std::string, std::string> >::iterator it = param_channels.begin(); it != param_channels.end(); ++it) {
		if (!isValidChannelName(it->first)) {
			sendToExecuter(ERR_NOSUCHCHANNEL(executer->getNickname(), it->first) + "\r\n");
			continue;
		}

		if (it->first.size() <= 1) {
			sendToExecuter(ERR_NOSUCHCHANNEL(executer->getNickname(), it->first) + "\r\n");
			continue;
		}

		
		Channel * channel = serverData->getChannelByName(it->first);
		
		// Check if user is already on the channel
		if (channel && channel->getMemberByNick(executer->getNickname())) {
			continue; // Already on channel, skip
		}
		
		if (channel == NULL) {
			try {
				Channel* new_channel = new Channel(it->first);
				if (!it->second.empty()) {
					new_channel->setMode(MODE_KEY);
					new_channel->setPassword(it->second);
				}
				new_channel->setOperator(executer);
				serverData->setChannel(new_channel);
				channel = new_channel;
				executer->addChannel(channel);
			} catch (std::bad_alloc& e) {
				std::cerr << "Memory allocation failed: " << e.what() << std::endl;
				continue;
			}
		}
		else {
			if (channel->hasMode(MODE_INVITE)) {
				if (!channel->isInvited(executer->getNickname())) {
					sendToExecuter(ERR_INVITEONLYCHAN(executer->getNickname(), it->first) + "\r\n");
					continue;
				}
			}
			if (channel->hasMode(MODE_KEY)) {
				if (!channel->verifyPassword(it->second)) {
					sendToExecuter(ERR_BADCHANNELKEY(executer->getNickname(), it->first) + "\r\n");
					continue;
				}
			}
			if (channel->hasMode(MODE_LIMIT)) {
				if (channel->isLimitReached()) {
					sendToExecuter(ERR_CHANNELISFULL(it->first) + "\r\n");
					continue;
				}
			}
			channel->setVoice(executer);
			executer->addChannel(channel);
		}
		
		// Send JOIN confirmation to the user
		std::string join_msg = ":" + executer->getNickname() + "!" + executer->getUsername() + "@" + executer->getIp() + " JOIN " + it->first + "\r\n";
		sendToExecuter(join_msg);
		
		// Send channel topic if exists
		// For now, assume no topic is set
		sendToExecuter("331 " + executer->getNickname() + " " + it->first + " :No topic is set\r\n");
		
		// Send member list (RPL_NAMREPLY)
		std::string member_list = "353 " + executer->getNickname() + " = " + it->first + " :";
		member_list += executer->getNickname(); // Add self to list
		sendToExecuter(member_list + "\r\n");
		
		// Send end of names
		sendToExecuter("366 " + executer->getNickname() + " " + it->first + " :End of /NAMES list\r\n");
	}
}

bool isValidChannelName(const std::string& channel_name) {

    if (channel_name.empty() || channel_name.length() > 50)
        return false;

    if (channel_name[0] != '#' && channel_name[0] != '&')
        return false;


    for (size_t i = 1; i < channel_name.length(); ++i) {
        char c = channel_name[i];

        if (isspace(c) || c == 0x07 /* BEL */ || c == ',' )
            return false;

        if (c < 0x21 || c > 0x7E)
            return false;
    }

    return true;
}
