#include "../includes/ServerData.hpp"

ServerData::ServerData(std::vector<struct pollfd>& poll_fds) : poll_fds(poll_fds){}

ServerData::~ServerData() {
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

void ServerData::removeChannel(Channel *target) {
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (*it == target) {
			delete *it;
			channels.erase(it);
			break;
		}
	}
}

bool ServerData::isValidChannel(Channel *channel) {
	if (channel == NULL)
		return false;

	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (*it == channel) {
			return true;
		}
	}
	return false;
}

void ServerData::enablePollOut(int client_fd) {
    for (size_t i = 0; i < this->poll_fds.size(); ++i) {
        if (poll_fds[i].fd == client_fd) {
            poll_fds[i].events |= POLLOUT;
            break;
        }
    }
}
