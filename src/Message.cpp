#include "../includes/Message.hpp"
#include "../includes/irc.hpp"

Message::Message() : command(""), prefix(""), error("") {}

void dupServer::parsing(std::string &request) { 
	Message message;
	std::string command_names [] = {};
	void (*commands[])(Message) = {}; //commad系の関数　

	message = tokenizeMessage(request);

	for (int i = 0; i < 0 ; i++){
		if (message.command == command_names[i]) {
			return (*commands[i])(message);
		}
	}

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

static void trimCRLF(std::string& str) {
	if (str.size() >= 2 && str[str.size() - 2] == '\r' && str[str.size() - 1] == '\n') {
		str.erase(str.size() - 2);
	} else if (!str.empty() && str[str.size() - 1] == '\n') {
		str.erase(str.size() - 1);
	}
}

Message tokenizeMessage(std::string &request) {
	Message message;
	std::istringstream ss(request);
	std::string token;
	int token_count = 0;
	// bool isPrefix = false;

	while (std::getline(ss, token, ' ')) {
		if (token.empty()) continue;

		if (token_count == 0  && token[0] == ':') {
			trimCRLF(token);
			message.prefix = token;
			// isPrefix = true;
			token_count ++;
			continue ;
		}
		trimCRLF(token);
		message.command = token;
		token_count ++;
		break;
	}
	//一応一旦分けました、、
	while (std::getline(ss, token, ' ')) {
		if (token.empty()) continue;

		if (token[0] == ':') {
			std::string trailing = token.substr(1);

			std::string rest;
			while (std::getline(ss, token, ' ')) {
				trailing += " " + token;
			}

			if (!trailing.empty()) {
				trimCRLF(trailing);
			}

			message.params.push_back(trailing);
			break;
		} else {
			trimCRLF(token);
			message.params.push_back(token);
		}
	}

	if (message.params.size() > 15)
		message.error = "number of param should be lower than 15";
	return message;
}
