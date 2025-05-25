#include "ServerData.hpp"

ServerData::ServerData() {
	// 必要なら初期化
}

ServerData::~ServerData() {
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		delete *it;
	}
	clients.clear();
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
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getNickname() == nick) {
			return *it;
		}
	}
	return NULL;
}
