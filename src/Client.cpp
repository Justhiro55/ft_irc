#include "../includes/Client.hpp"

Client::Client() : fd(-1), ip(""), port(0), auth(false), isRegister(false), welcomeSent(false), disconnected(false), nickname(""), host(""), username(""), server(""), modes(0) {}

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
	return this->modes & mode;
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

void Client::setDisconnected(bool disconnected) {
    this->disconnected = disconnected;
}

bool Client::isDisconnected() const {
    return this->disconnected;
}

void Client::clearChannels() {
    this->join_channels.clear();
}

void Client::addChannel(Channel* channel) {
    this->join_channels.push_back(channel);
}

std::vector<Channel*>& Client::getChannels() {
    return this->join_channels;
}

void Client::removeChannel(Channel *channel) {
    for (std::vector<Channel *>::iterator it = join_channels.begin(); it != join_channels.end(); ++it) {
        if (*it == channel) {
            it = join_channels.erase(it);
            return;
        }
    }
}

const std::string& Client::getHostname() const {
    return this->ip;
}
