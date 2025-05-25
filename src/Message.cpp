#include "Message.hpp"
#include "irc.hpp"


void dupServer::parsing(std::string &request) { 
	Message message;
	std::string command_names [] = {};
	void (*commands[])(Message) = {}; //commad系の関数　

	message = splitMessage(request);

	for (int i = 0; i < 0 ; i++){
		if (message.command == command_names[i]) {
			return (*commands[i])(message);
		}
	}
	//error not found
}


Message& Message::operator=(const Message& obj) {
	if (this != &obj) { // 自己代入のチェック
		this->params = obj.params;
		this->command = obj.command;
		this->prefix = obj.prefix;
		this->error = obj.error;
	}
	return *this;
}

Message splitMessage(std::string &request) {
	Message message;
	std::istringstream ss(request);
	std::string token;
	int token_count = 0;
	bool isPrefix = false;

	while (std::getline(ss, token, ' ')) {
		if (token.empty()) continue;

		if (token_count == 0  && token[0] == ':') {
			message.prefix = token;
			isPrefix = true;
			token_count ++;
			continue ;
		}
		message.command = token;
		token_count ++;
		break;
	}
	//一応一旦分けました、、
	while (std::getline(ss, token, ' ')) {
		if (token.empty()) continue;
		message.params.push_back(token);
	}

	if (message.params.size() > 15)
		message.error = "number of param should be lower than 15";
	return message;
}