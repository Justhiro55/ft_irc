#pragma once

#include <vector>
#include <iostream>
#include <sstream>

class Message {
	public: //ほぼ構造体的な使い方したいためpublic
		std::vector<std::string> params;
		std::string command;
		std::string prefix;
		std::string error; // ここは一旦適当

   		Message& operator=(const Message& obj);
};