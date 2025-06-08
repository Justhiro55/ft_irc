#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) : name(name){}

Channel::~Channel() {}

std::string Channel::getName() {
	return this->name;
}

void Channel::setOperator(Client *member) {
	members.insert(std::make_pair(member, 'o'));
}

void Channel::setVoice(Client *member) {
	members.insert(std::make_pair(member, 'v'));
}

