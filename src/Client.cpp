#include "Client.hpp"

#include "Client.hpp"

Client::Client() : fd(-1), nickname(""), host(""), username(""), server("") {}

Client::~Client() {}

void Client::setClientFd(int fd) {
	this->fd = fd;
}

void Client::setAuth(bool auth) {
	this->auth = auth;
}

void Client::setNickname(std::string &nickname) {
    this->nickname = nickname;
}

void Client::setHost(std::string &host) {
    this->host = host;
}

void Client::setUsername(std::string &username) {
    this->username = username;
}

void Client::setServer(std::string &server) {
    this->server = server;
}

int Client::getClientFd() {
    return this->fd;
}

bool Client::getAuth() {

}

std::string Client::getNickname() {
    return this->nickname;
}

std::string Client::getHost() {
    return this->host;
}

std::string Client::getUsername() {
    return this->username;
}

std::string Client::getServer() {
    return this->server;
}

ssize_t Client::pushToRecvQueue() {
	char buffer[1024];
	ssize_t bytes_received = recv(this->fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received <= 0)
		return bytes_received;
	
	this->receiveBuffer.append(buffer);
	std::queue<std::string> messages = splitStream(receiveBuffer, "/r/n");
	while (messages.size()) {
		this->recvQueue.push(tokenizeMessage(messages.front()));
		messages.pop();
	}

	return bytes_received;
}

std::queue<std::string> Client::splitStream(std::string& val, const std::string& delim) {
    std::queue<std::string> ret;
    size_t pos = 0;

    while ((pos = val.find(delim)) != std::string::npos) {
        std::string word = val.substr(0, pos);

        if (word != "")
            ret.push(val.substr(0, pos));

        val.erase(0, pos + delim.length());
    }
    return ret;
}

