#include "AbstractCommand.hpp"

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

