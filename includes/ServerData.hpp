#pragma once

#include "Client.hpp"
#include "Channel.hpp"
#include "vector"
#include <poll.h>

#define OPER_NAME     "admin"
#define OPER_PASSWORD "secretpass"

class ServerData {
	private:
		std::vector<Client *> clients;
		std::vector<Channel *> channels;
		std::string password;
		std::vector<struct pollfd>& poll_fds;

	public:
		ServerData(std::vector<struct pollfd>& poll_fds);
		~ServerData();

		void setClient(Client *client);
		void setChannel(Channel *channel);
		void setPassword(std::string password);
		void removeClient(Client *client);

		bool	verifyPassword(std::string password);

		Client	*getClientByFd(int fd);
		Client	*getClientByNickname(std::string &nick);

		Channel	*getChannelByName(std::string &name);
		void removeChannel(Channel *target);

		void enablePollOut(int client_fd);
};