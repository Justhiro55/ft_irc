
#pragma once

#include "map"
#include "vector"
#include "string"
#include "iostream"

#define MODE_INVITE     (1 << 0)   // 00001   +i (招待制限)  
#define MODE_TOPICLOCK  (1 << 1)   // 00010   +t（トピック固定）
#define MODE_KEY        (1 << 2)   // 00100   +k（鍵付き）
#define MODE_LIMIT      (1 << 3)   // 01000   +l（人数制限あり）
#define MODE_OPERATOR	(1 << 4)   // 10000   +o (オペレーター権限付与)

class Client ;

class Channel {
	private:
		std::string name;
		std::map<Client *, unsigned char> members;
		std::vector<std::string> invite_list;
		std::string	topic;
		int	members_limit;
		std::string	password;
		unsigned short modes;
	public:
		Channel();
		Channel(std::string name);
		~Channel();
		std::string getName();

		Client* getMemberByNick(const std::string &nick) const;
		void setPassword(std::string password);
		void unsetMember(Client *member);
		void setOperator(Client *member);
		void setVoice(Client *member);
		void setMode(unsigned short mode);
		void addInvite_list(const std::string &invitee);
		bool isInvited(const std::string &nickname) const;
		void unsetMode(unsigned short mode);
		bool hasMode(unsigned short mode);
		bool verifyPassword(const std::string &password) const;
};