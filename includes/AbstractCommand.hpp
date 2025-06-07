#pragma once

#include <iostream>
#include "Message.hpp"
#include "Client.hpp"
#include "ServerData.hpp"

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

		// serverDataのgetterとsetter
		ServerData* getServerData() const;
		void setServerData(ServerData* serverData);

		// executerのgetterとsetter
		Client* getExecuter() const;
		void setExecuter(Client* executer);
};
