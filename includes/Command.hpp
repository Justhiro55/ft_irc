#pragma once

#include "AbstractCommand.hpp"

#include <string>
#include <cctype>

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

class Join : public AbstractCommand {

};
