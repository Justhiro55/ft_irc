#include "Client.hpp"

#include "Client.hpp"

Client::Client() : fd(-1), nickname(""), host(""), username(""), server("") {}

Client::~Client() {}

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
