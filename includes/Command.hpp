#pragma once

#include "AbstractCommand.hpp"

#include <string>
#include <cctype>
#include <sstream> 
#include <utility>

class Nick : public AbstractCommand {
	public:
		Nick();
		~Nick();

		void executeCmd();
		bool is_valid_nickname(const std::string& nick);
};

class Pass : public AbstractCommand {

	public:
		Pass();
		~Pass();
		
		void executeCmd();
};

class User : public AbstractCommand {
	public:
		User();
		~User();

		void executeCmd();
};

class Oper :public AbstractCommand {
	public:
		Oper();
		~Oper();

		void executeCmd();
};

class Join : public AbstractCommand {
	public:
		Join();
		~Join();

		void executeCmd();
};

class Mode : public AbstractCommand {
	public:
		Mode();
		~Mode();

		void executeCmd();
		std::string setModes(Client *target);
		std::string setModes(Channel *target);
		std::string getMode(Client* client);
		std::string getMode(Channel* channel);
};
int atoi_checked(std::string literal);

class Topic : public AbstractCommand {
	public:
		Topic();
		~Topic();

		void executeCmd();
};

class Privmsg : public AbstractCommand {
	public:
		Privmsg();
		~Privmsg();

		void executeCmd();
		void removeDuplicates(std::vector<std::string>& vec);
};

class Notice : public AbstractCommand {
	public:
		Notice();
		~Notice();
	
		void executeCmd();
		void removeDuplicates(std::vector<std::string>& vec);
};

class Part : public AbstractCommand {
	public:
		Part();
		~Part();

		void executeCmd();
};

