
#pragma once

#include "Client.hpp"
#include "map"

class Channel {
	private:
		std::string name;
		std::map<Client *, unsigned char> members;
		std::vector<std::string> invite_list;
		std::string	password;
	public:
		Channel();
		Channel(std::string name);
		~Channel();
		std::string getName();

		void setOperator(Client *member);
		void setVoice(Client *member);
};