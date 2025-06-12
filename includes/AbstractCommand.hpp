#pragma once

#include <iostream>
#include "Message.hpp"
#include "Client.hpp"
#include "ServerData.hpp"
#include "Reply.hpp"

class AbstractCommand {
	protected:
		Message message;
		ServerData *serverData;
		Client	*executer;

	public:
		AbstractCommand();
		virtual ~AbstractCommand() = 0;
		virtual void executeCmd() = 0;

		const Message& getMessage() const;
		void setMessage(const Message& message);

		ServerData* getServerData() const;
		void setServerData(ServerData* serverData);

		Client* getExecuter() const;
		void setExecuter(Client* executer);

		void sendToExecuter(const std::string &reply);
		void sendToClient(Client *client, std::string &reply);
		void sentToClients(std::vector<Client *> clients, std::string &reply);
};
