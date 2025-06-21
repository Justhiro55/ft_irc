#include "../includes/Command.hpp"

Quit::Quit() : AbstractCommand(){}

Quit::~Quit() {}

void Quit::executeCmd() {
	std::string quitMessage = "Quit: ";

	if (!message.params.empty() && !message.params[0].empty()) {
		quitMessage += message.params[0];
	} else {
		quitMessage += executer->getNickname();
	}

	std::vector<Client*> clientsToNotify;
	std::vector<Channel*> channels;

	for (size_t i = 0; i < executer->getChannels().size(); i++) {
		Channel* channel = executer->getChannels()[i];
		if (!this->serverData->isValidChannel(channel)) {
			continue;
		}
		channels.push_back(channel);

		// チャンネル内の全クライアント取得
		std::vector<Client*> channelClients = channel->getClients();

		for (size_t j = 0; j < channelClients.size(); j++) {
			Client* client = channelClients[j];
			if (client != executer) {
				bool alreadyAdded = false;
				for (size_t k = 0; k < clientsToNotify.size(); k++) {
					if (clientsToNotify[k] == client) {
						alreadyAdded = true;
						break;
					}
				}
				if (!alreadyAdded) {
					clientsToNotify.push_back(client);
				}
			}
		}
	}

	// : <executer_nickname>!<executer_username>@<executer_hostname> QUIT :<quit_message>
	std::string quitReply = ":" + executer->getNickname() + "!" + executer->getUsername() + "@" + executer->getHostname() + " QUIT :" + quitMessage + "\r\n";

	for (size_t i = 0; i < clientsToNotify.size(); i++) {
		std::string tempReply = quitReply;
		this->sendToClient(clientsToNotify[i], tempReply);
	}

	for (size_t i = 0; i < channels.size(); i++) {
		channels[i]->removeClient(executer);
		if (!channels[i]->members_size())
		{
			serverData->removeChannel(channels[i]);
			channels[i] = NULL;
		}
	}

	executer->clearChannels();

	executer->setDisconnected(true);
}
