#include "../includes/Command.hpp"

Join::Join() {}

Join::~Join() {}

void Join::executeCmd() {
	std::string channel_name;
	std::string key;
	if (this->executer->getAuth())
		return ; // 484  ERR_RESTRICTED ":Your connection is restricted!"
	
	if (message.params.size() <= 0)
		return ; // 461 ERR_NEEDMOREPARAMS  <command> :Not enough parameters

	for (std::vector<std::string>::iterator it = message.params.begin(); it != message.params.end(); ++it) {
		std::stringstream ss(*it);
		std::string channel_name;
		std::string key;
		
		std::getline(ss, channel_name, ',');
		std::getline(ss, key);

		if (!isValidChannelName(channel_name))
			continue;
			
		Channel * channel = serverData->getChannelByName(channel_name);
		if (channel == NULL) {
			try {
				Channel* new_channel = new Channel(channel_name);
				// if (!key.empty())
				// 	channle->modeChange
				new_channel->setOperator(executer);
				serverData->setChannel(new_channel);
			} catch (std::bad_alloc& e) {
				std::cerr << "メモリ確保に失敗しました: " << e.what() << std::endl;
			}
		}
		else {
			channel->setVoice(executer);
		}
	}
}

bool isValidChannelName(const std::string& channel_name) {

    if (channel_name.empty() || channel_name.length() > 50)
        return false;

    if (channel_name[0] != '#')
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
