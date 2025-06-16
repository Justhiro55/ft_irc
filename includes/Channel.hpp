
#pragma once

#include "map"
#include "vector"
#include "string"
#include "iostream"
#include "Client.hpp"

#define MODE_INVITE     (1 << 0)   // 00001   +i (招待制限)  
#define MODE_TOPICLOCK  (1 << 1)   // 00010   +t（トピック固定）
#define MODE_KEY        (1 << 2)   // 00100   +k（鍵付き）
#define MODE_LIMIT      (1 << 3)   // 01000   +l（人数制限あり）

class Client ;

class Channel {
	private:
		std::string name;
		std::map<Client *, unsigned char> members;
		std::vector<std::string> invite_list;
		std::string	topic;
		size_t members_limit;
		std::string	password;
		unsigned short modes;
	public:
		Channel();
		Channel(std::string name);
		~Channel();
		std::string getName();

		Client* getMemberByNick(const std::string &nick) const;

		unsigned char getMemberMode(const std::string &nick) const;
		void setMemberMode(const std::string &nick, unsigned char mode);
		bool isOperator(Client *member) const;
		void setPassword(std::string password);
		void unsetMember(Client *member);
		bool isMember(const std::string &nick);

		void sendToMembers(const std::string &reply, const std::string &excluded_nick);

		void setPassword(std::string &password);
		void unsetPassword();
		bool isPasswordSet() const;

		void setOperator(Client *member);
		bool isOperatorMember(const std::string &nick);
		void setVoice(Client *member);

		void addInvite_list(const std::string &invitee);
		bool isInvited(const std::string &nickname) const;

		void setLimit(size_t limit);

		void setMode(unsigned short mode);
		void unsetMode(unsigned short mode);
		bool hasMode(unsigned short mode);

		bool verifyPassword(const std::string &password) const;

		std::string getTopic() const;
		void setTopic(std::string topic);
		void clearTopic();
};