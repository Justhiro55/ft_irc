#include "../includes/Client.hpp"

Client::Client() : fd(-1), ip(""), port(0), auth(false), isRegister(false), welcomeSent(false), nickname(""), host(""), username(""), server("") {}

Client::~Client() {}

void Client::setClientFd(int fd) {
	this->fd = fd;
}

void Client::setAuth(bool auth) {
	this->auth = auth;
}

void Client::setRegister(bool isRegister) {
	this->isRegister = isRegister;
}

void Client::setWelcomeSent(bool sent) {
	this->welcomeSent = sent;
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

void Client::setRealname(std::string &realname) {
	this->realname = realname;
}

void Client::setIp(const std::string& ip) {
    this->ip = ip;
}

void Client::setPort(int port) {
    this->port = port;
}

void Client::setMode(unsigned short mode) {
	if (mode == USER_MODE_OPERATOR)
		this->modes |= USER_MODE_OPERATOR;
	
	return;
}

void Client::unsetMode(unsigned short mode) {
	if (mode == USER_MODE_OPERATOR)
		this->modes &= ~USER_MODE_OPERATOR;

	return ;
}

bool Client::hasMode(unsigned short mode) {
	if (mode == USER_MODE_OPERATOR)
		return true;
	return false;
}

int Client::getClientFd() {
    return this->fd;
}

bool Client::getAuth() {
    return this->auth;
}

bool Client::getRegister() {
    return this->isRegister;
}

bool Client::getWelcomeSent() {
    return this->welcomeSent;
}

std::string Client::getNickname() const {
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

const std::string& Client::getIp() const {
    return this->ip;
}

int Client::getPort() const {
    return this->port;
}

std::string& Client::getRealname() const {
    return const_cast<std::string&>(this->realname);
}

bool Client::isFullyRegistered() {
    return this->auth && !this->nickname.empty() && !this->username.empty() && this->isRegister;
}

bool Client::checkAndCompleteRegistration() {
    // Check if all registration requirements are met
    bool wasRegistered = this->isRegister;
    bool isNowFullyRegistered = this->auth && !this->nickname.empty() && !this->username.empty();
    
    if (isNowFullyRegistered && !wasRegistered) {
        this->isRegister = true;
        return true; // Registration just completed
    }
    
    return false; // Registration was already complete or not yet complete
}

void Client::pushToSendQueue(std::string reply) {
    this->sendQueue.push(reply);
}

void Client::pushMessageToRecvQueue(const Message& message) {
    this->recvQueue.push(message);
}

ssize_t Client::pushToRecvQueue() {
	char buffer[1024];
	ssize_t bytes_received = recv(this->fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received <= 0)
		return bytes_received;
	
	this->receiveBuffer.append(buffer);
	std::queue<std::string> messages = splitStream(receiveBuffer, "\r\n");
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
