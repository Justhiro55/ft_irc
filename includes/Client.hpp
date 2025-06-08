#pragma once

#include "Message.hpp"
#include "Channel.hpp"

#include <iostream>
#include <queue>
#include <sys/socket.h>

class Client {
	private:
		int		fd;
		bool	auth;
		bool	isRegister;
		std::string nickname;
		std::string host;
		std::string username;
		std::string server;

		std::vector<Channel *> join_channels; 

		std::string receiveBuffer;
		std::queue<Message>		recvQueue;
		std::queue<std::string> sendQueue;

	public:
		Client();
		~Client();
		void setClientFd(int fd);
		void setNickname(std::string &nickname);
		void setHost(std::string &host);
		void setUsername(std::string &server);
		void setServer(std::string &server);
		void setAuth(bool auth);

		ssize_t pushToRecvQueue();
		void pushToSendQueue(std::string reqly);
		std::queue<std::string> splitStream(std::string& val, const std::string& delim);

		int getClientFd();
		bool getAuth();
		std::string getNickname();
		std::string getHost();
		std::string getUsername();
		std::string getServer();
};