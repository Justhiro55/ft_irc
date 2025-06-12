#pragma once

#include "Client.hpp"
#include "Channel.hpp"
#include "vector"

class ServerData {
	private:
		std::vector<Client *> clients;
		std::vector<Channel *> channels;
		std::string password;

	public:
		ServerData();
		~ServerData();

		void setClient(Client *client);
		void setChannel(Channel *channel);
		void setPassword(std::string password);
		void removeClient(Client *client);

		bool	verifyPassword(std::string password);

		Client	*getClientByFd(int fd);
		Client	*getClientByNickname(std::string &nick);

		Channel	*getChannelByName(std::string &name);
};