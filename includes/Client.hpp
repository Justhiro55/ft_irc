#pragma once

#include "Message.hpp"
#include "Channel.hpp"

#include <iostream>
#include <queue>
#include <sys/socket.h>
#include <string>
#include <cctype>
#include <sstream>
#include <utility>
#include "map"

#define USER_MODE_OPERATOR	(1 << 0)   // 0001   +o (オペレーター権限付与)

class Channel;

class Client {
	private:
		int		fd;
		std::string ip;
		int port;
		bool	auth;
		bool	isRegister;
		bool	welcomeSent;
		bool	disconnected;
		std::string nickname;
		std::string host;
		std::string username;
		std::string server;
		std::string realname;
		std::vector<Channel *> join_channels;
		unsigned short modes;

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
		void setRealname(std::string &realname);
		void setAuth(bool auth);
		void setRegister(bool isRegister);
		void setWelcomeSent(bool sent);
		void setMode(unsigned short mode);
		void unsetMode(unsigned short mode);
		bool hasMode(unsigned short mode);
		ssize_t pushToRecvQueue();
		void pushMessageToRecvQueue(const Message& message);
		void pushToSendQueue(std::string reply);
		std::queue<std::string> splitStream(std::string& val, const std::string& delim);
		bool isFullyRegistered();
		bool checkAndCompleteRegistration();

		int getClientFd();
		bool getAuth();
		bool getRegister();
		bool getWelcomeSent();
		std::string getNickname() const;
		std::string getHost();
		std::string getUsername();
		std::string getServer();
		std::string& getRealname() const;

		void setIp(const std::string& ip);
		void setPort(int port);
		const std::string& getIp() const;
		int getPort() const;

		std::string& getBuffer() { return receiveBuffer; }
		const std::string& getBuffer() const { return receiveBuffer; }

		std::queue<std::string>& getSendQueue() { return sendQueue; }
		std::queue<Message>& getRecvQueue() { return recvQueue; }

		void setDisconnected(bool disconnected);
		bool isDisconnected() const;
		void clearChannels();
		void addChannel(Channel* channel);
		std::vector<Channel*>& getChannels();
		const std::string& getHostname() const;
};
