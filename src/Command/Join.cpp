#include "../includes/Command.hpp"
#include "map"

Join::Join() :AbstractCommand() {}

Join::~Join() {}

void Join::executeCmd() {
	size_t params_size = message.params.size();
	std::vector<std::pair<std::string, std::string>> param_channels;
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return ; // 484  ERR_RESTRICTED ":Your connection is restricted!"
	
	if (params_size < 1 || params_size > 2)
		return ; // 461 ERR_NEEDMOREPARAMS  <command> :Not enough parameters

	std::stringstream ss(message.params[0]);
	std::string channel_name;

	while (std::getline(ss, channel_name, ',')) {
		param_channels.push_back(std::make_pair(channel_name, ""));
	}

	if (params_size == 2) {
		std::stringstream ss(message.params[1]);
		std::string key;
		std::vector<std::pair<std::string, std::string>>::iterator it = param_channels.begin();

		while (std::getline(ss, key, ',')) {
			if (it == param_channels.end())
				break;
			it->second = key;
			++it;
		}
	}

	for(std::vector<std::pair<std::string, std::string>>::iterator it = param_channels.begin(); it != param_channels.end(); ++it) {
		if (!isValidChannelName(it->first))
			continue ; // error
		it->first.erase(0, 1);
		Channel * channel = serverData->getChannelByName(it->first);
		if (channel == NULL) {
			try {
				Channel* new_channel = new Channel(channel_name);
				if (!it->second.empty()) {
					new_channel->setMode(MODE_KEY);
					new_channel->setPassword(it->second);
				}
				new_channel->setOperator(executer);
				serverData->setChannel(new_channel);
			} catch (std::bad_alloc& e) {
				std::cerr << "メモリ確保に失敗しました: " << e.what() << std::endl;
			}
		}
		else {
			if (channel->hasMode(MODE_INVITE)) {
				if (!channel->isInvited(executer->getNickname()))
					continue; //err code
			}
			if (channel->hasMode(MODE_KEY)) {
				if (!channel->verifyPassword(it->second))
					continue; // err code
			}
			channel->setVoice(executer);
		}
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
