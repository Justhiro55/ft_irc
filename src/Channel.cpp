#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) : name(name), members_limit(0), modes(0) {
	(void)members_limit;
}

Channel::~Channel() {}

std::string Channel::getName() {
	return this->name;
}

void Channel::setPassword(std::string password) {
	this->password = password;
}

bool Channel::verifyPassword(const std::string &password) const {
	return this->password == password;
}

void Channel::unsetMember(Client *member) {
	members.erase(member);
}

void Channel::addInvite_list(const std::string &invitee) {
	if (std::find(invite_list.begin(), invite_list.end(), invitee) == invite_list.end()) {
		invite_list.push_back(invitee);
	}
}

bool Channel::isInvited(const std::string &nickname) const {
	return std::find(invite_list.begin(), invite_list.end(), nickname) != invite_list.end();
}

void Channel::setOperator(Client *member) {
	members.insert(std::make_pair(member, 'o'));
}

void Channel::setVoice(Client *member) {
	members.insert(std::make_pair(member, 'v'));
}

Client* Channel::getMemberByNick(const std::string &nick) const {
	for (std::map<Client*, unsigned char>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (it->first->getNickname() == nick)
			return it->first;
	}
	return NULL;
}

bool Channel::isOperator(Client *member) const {
	std::map<Client*, unsigned char>::const_iterator it = members.find(member);
	if (it != members.end()) {
		return it->second == 'o';
	}
	return false;
}

void Channel::setMode(unsigned short mode) {
	if (mode == MODE_INVITE)
		this->modes |= MODE_INVITE;
	else if (mode == MODE_TOPICLOCK)
		this->modes |= MODE_TOPICLOCK;
	else if (mode == MODE_KEY)
		this->modes |= MODE_KEY;
	else if (mode == MODE_LIMIT)
		this->modes |= MODE_LIMIT;
	
	return;
}

void Channel::unsetMode(unsigned short mode) {
	if (mode == MODE_INVITE)
		this->modes &= ~MODE_INVITE;
	else if (mode == MODE_TOPICLOCK)
		this->modes &= ~MODE_TOPICLOCK;
	else if (mode == MODE_KEY)
		this->modes &= ~MODE_KEY;
	else if (mode == MODE_LIMIT)
		this->modes &= ~MODE_LIMIT;
	
	return ;
}

bool Channel::hasMode(unsigned short mode) {
	return this->modes & mode;
}
