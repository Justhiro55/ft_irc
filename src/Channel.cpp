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

void Channel::setPassword(std::string &password) {
	this->password = password;
}

void Channel::unsetPassword() {
	this->password.clear();
}

bool Channel::isPasswordSet() const {
	if (this->password.empty())
		return false;
	return true;
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
void Channel::removeInvite(const std::string &invitee) {
	std::vector<std::string>::iterator it = std::find(invite_list.begin(), invite_list.end(), invitee);
	if (it != invite_list.end()) {
		invite_list.erase(it);
	}
}

bool Channel::isInvited(const std::string &nickname) const {
	return std::find(invite_list.begin(), invite_list.end(), nickname) != invite_list.end();
}

void Channel::setLimit(size_t limit) {
	this->members_limit = limit;
}

bool Channel::isLimitReached() const {
	return this->members_limit <= countMembers();
}

void Channel::setOperator(Client *member) {
	members.insert(std::make_pair(member, 'o'));
}

bool Channel::isMember(const std::string &nick) {
	Client *member = getMemberByNick(nick);
	if (member == NULL)
		return false;
	return true;
}

bool Channel::isOperatorMember(const std::string &nick) {
	unsigned char mode = getMemberMode(nick);
	if (mode == 'o')
		return true;
	return false;
}

void Channel::setVoice(Client *member) {
	members.insert(std::make_pair(member, 'v'));
}

unsigned char Channel::getMemberMode(const std::string &nick) const {
	for (std::map<Client*, unsigned char>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (it->first->getNickname() == nick)
			return it->second;
	}
	return '\0';
}

void Channel::setMemberMode(const std::string &nick, unsigned char mode) {
	for (std::map<Client*, unsigned char>::iterator it = members.begin(); it != members.end(); ++it) {
		if (it->first->getNickname() == nick) {
			it->second = mode;
		}
	}
}

Client* Channel::getMemberByNick(const std::string &nick) const {
	for (std::map<Client*, unsigned char>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (it->first->getNickname() == nick)
			return it->first;
	}
	return NULL;
}

size_t Channel::countMembers() const  {
	return this->members.size();
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

void Channel::sendToMembers(const std::string &reply, const std::string &excluded_nick) {
	for (std::map<Client*, unsigned char>::iterator it = members.begin(); it != members.end(); ++it) {
		if (it->first->isDisconnected())
			continue;
		if (it->first->getNickname() == excluded_nick)
			continue;
		it->first->pushToSendQueue(reply);
	}
}

std::string Channel::getTopic() const {
	return this->topic;
}

void Channel::setTopic(std::string topic) {
	this->topic = topic;
}

void Channel::clearTopic() {
	this->topic.clear();
}

std::vector<Client*> Channel::getClients() const {
	std::vector<Client*> clients;
	for (std::map<Client*, unsigned char>::const_iterator it = members.begin(); it != members.end(); ++it) {
		clients.push_back(it->first);
	}
	return clients;
}

void Channel::removeClient(Client *client) {
	members.erase(client);
}

size_t Channel::members_size() {
	return this->members.size();
}