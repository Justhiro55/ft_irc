#pragma once

#include <vector>
#include <iostream>
#include <sstream>

class Message {
	public:
		std::vector<std::string> params;
		std::string command;
		std::string prefix;
		std::string error;
		
		Message();
   		Message& operator=(const Message& obj);
};

Message tokenizeMessage(std::string &request);