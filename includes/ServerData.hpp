#pragma once

#include "Client.hpp"
#include "vector"

class ServerData {
	private:
		std::vector<Client *> clients; 

	public:
		ServerData();
		~ServerData();

		Client *getClientByFd(int fd);
		Client *getClientByNickname(std::string &nick);
};