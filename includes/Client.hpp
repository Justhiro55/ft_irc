#pragma once

#include <iostream>

class Client {
	private:
		int		fd;
		bool	auth;
		std::string nickname;
		std::string host;
		std::string username;
		std::string server;	

	public:
		Client();
		~Client();
		void setNickname(std::string &nickname);
		void setHost(std::string &host);
		void setUsername(std::string &server);
		void setServer(std::string &server);

		int getClientFd();
		std::string getNickname();
		std::string getHost();
		std::string getUsername();
		std::string getServer();
};