#include "../includes/AbstractCommand.hpp"

AbstractCommand::AbstractCommand() : serverData(NULL), executer(NULL) {}

AbstractCommand::~AbstractCommand() {}

const Message& AbstractCommand::getMessage() const {
    return this->message;
}

void AbstractCommand::setMessage(const Message& message) {
    this->message = message;
}

ServerData* AbstractCommand::getServerData() const {
    return this->serverData;
}

void AbstractCommand::setServerData(ServerData* serverData) {
    this->serverData = serverData;
}

Client* AbstractCommand::getExecuter() const {
    return this->executer;
}

void AbstractCommand::setExecuter(Client* executer) {
    this->executer = executer;
}

void AbstractCommand::sendToExecuter(const std::string &reply) {
	if (reply.empty())
		return ;
	executer->pushToSendQueue(reply);
}

void AbstractCommand::sendToClient(Client *client, const std::string &reply) {
	if (reply.empty())
		return ;
	serverData->enablePollOut(client->getClientFd());
	client->pushToSendQueue(reply);
}

void AbstractCommand::sendToClients(std::vector<Client *> clients, const std::string &reply) {
	if (clients.size() <= 0 || reply.empty())
		return;
	
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
		this->sendToClient(*it, reply);
	}
}

void AbstractCommand::sendWelcomeMessages() {
	if (this->executer->getWelcomeSent()) {
		return; // Already sent welcome messages
	}

	std::string nick = this->executer->getNickname();
	std::string user = this->executer->getUsername();
	std::string host = this->executer->getIp();

	std::string welcome = RPL_WELCOME(nick, user, host) + "\r\n";
	std::string yourhost = RPL_YOURHOST(nick, "ft_irc", "1.0") + "\r\n";
	std::string created = RPL_CREATED(nick, "Today") + "\r\n";
	std::string myinfo = RPL_MYINFO(nick, "ft_irc", "1.0", "o", "itkol") + "\r\n"; // Todo: usermode, channnel mode 仮置き

	this->sendToExecuter(welcome);
	this->sendToExecuter(yourhost);
	this->sendToExecuter(created);
	this->sendToExecuter(myinfo);

	this->executer->setWelcomeSent(true);
}
