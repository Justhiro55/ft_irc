#include "../includes/ServerData.hpp"

ServerData::ServerData() {
	// 必要なら初期化
}

ServerData::~ServerData() {
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		delete *it;
	}

	for (std::vector<Channel*>::iterator it =  channels.begin(); it != channels.end(); ++it) {
		delete *it;
	}

	clients.clear();
}


void ServerData::setClient(Client *client) {
	clients.push_back(client);
}

void ServerData::removeClient(Client *client) {
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (*it == client) {
			clients.erase(it);
			break;
		}
	}
}

void ServerData::setPassword(std::string password) {
	this->password = password;
}

bool ServerData::verifyPassword(std::string password) {
	return this->password == password;
}

Client* ServerData::getClientByFd(int fd) {
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getClientFd() == fd) {
			return *it;
		}
	}
	return NULL;
}

Client* ServerData::getClientByNickname(std::string &nick) {
	if (nick.empty())
		return NULL;

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getNickname() == nick) {
			return *it;
		}
	}
	return NULL;
}


void ServerData::setChannel(Channel *channel) {
	if (channel == NULL)
		return;
	this->channels.push_back(channel);
}

Channel* ServerData::getChannelByName(std::string &name) {
	if (name.empty())
		return NULL;

	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if ((*it)->getName() == name) {
			return *it;
		}
	}
	return NULL;
}