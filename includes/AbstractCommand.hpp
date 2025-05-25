#pragma once

#include <iostream>
#include "Message.hpp"
#include "Client.hpp"
#include "ServerData.hpp"

class AbstractCommand {
	private:
		Message message;
		ServerData *serverData;
		Client	*client;

	public:
		AbstractCommand();
		virtual ~AbstractCommand() = 0;
		virtual void executeCmd() = 0;
		
};
