#pragma once

#include "Message.hpp"
#include <iostream>
#include <queue>
#include <sys/socket.h>

class Client {
	private:
		int		fd;
		std::string ip;
		int port;
		bool	auth;
		bool	isRegister;
		std::string nickname;
		std::string host;
		std::string username;
		std::string server;
		std::string receiveBuffer;
		std::queue<Message>		recvQueue;
		std::queue<std::string> sendQueue;

	public:
		Client();
		~Client();
		void setClientFd(int fd);
		void setNickname(std::string &nickname);
		void setHost(std::string &host);
		void setUsername(std::string &username);
		void setServer(std::string &server);
		void setAuth(bool auth);
		void setRegister(bool isRegister);

		ssize_t pushToRecvQueue();
		void pushToSendQueue(std::string reply);
		std::queue<std::string> splitStream(std::string& val, const std::string& delim);

		int getClientFd();
		bool getAuth();
		bool getRegister();
		std::string getNickname();
		std::string getHost();
		std::string getUsername();
		std::string getServer();

		void setIp(const std::string& ip);
		void setPort(int port);
		const std::string& getIp() const;
		int getPort() const;

		std::string& getBuffer() { return receiveBuffer; }
		const std::string& getBuffer() const { return receiveBuffer; }

		std::queue<std::string>& getSendQueue() { return sendQueue; }
};
