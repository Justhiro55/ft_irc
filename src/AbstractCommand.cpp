#include "../includes/AbstractCommand.hpp"

AbstractCommand::AbstractCommand() : serverData(NULL), executer(NULL) {}

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

void AbstractCommand::sendToClient(Client *client, std::string &reply) {
	if (reply.empty())
		return ;
	client->pushToSendQueue(reply);
}

void AbstractCommand::sentToClients(std::vector<Client *> clients, std::string &reply) {
	if (clients.size() <= 0 || reply.empty())
		return;
	
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
		this->sendToClient(*it, reply);
	}
}
