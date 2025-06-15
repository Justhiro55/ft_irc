#include "Command.hpp"

Mode::Mode() :AbstractCommand() {}

Mode::~Mode() {}

void Mode::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return sendToExecuter(ERR_NOTREGISTERED(executer->getNickname()) + "\r\n");

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size < 1)
		return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "MODE") + "\r\n");

    if ( params.front()[0] == '#' || params.front()[0] == '&' ) {
        Channel* channel;
		std::string channel_name = params.front();

		if ( channel_name.size() <= 1 )
            return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "MODE") + "\r\n");

        channel = serverData->getChannelByName(channel_name);
		if (channel == NULL)
			return sendToExecuter(ERR_NOSUCHCHANNEL(channel_name) + "\r\n");
		if (!channel->isMember(executer->getNickname()))
			return sendToExecuter(ERR_NOTONCHANNEL(channel->getName()) + "\r\n");
		if (params_size == 1)
			return sendToExecuter(RPL_CHANNELMODEIS(executer->getNickname(), channel_name, getMode(channel)) + "\r\n");

		if (channel->isOperatorMember(executer->getNickname()))
			return sendToExecuter(ERR_CHANOPRIVSNEEDED(channel_name) + "\r\n");

		std::string applied_modes = setModes(channel);
		if (applied_modes.empty() || applied_modes.size() == 1)
			return;
		channel->sendToMembers(RPL_CHANNELMODECHANGE(executer->getNickname(), executer->getUsername(), executer->getHost(), channel_name, applied_modes), "");
    }
	else {
		Client* client;
		std::string client_nick = params.front();

		if (client_nick.empty())
			return sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), "MODE") + "\r\n");

		client = serverData->getClientByNickname(client_nick);
		if (client == NULL)
			return sendToExecuter(ERR_NOSUCHNICK(client_nick) + "\r\n");
		if (client != executer)
			return sendToExecuter(ERR_USERSDONTMATCH(executer->getNickname()) + "\r\n");
		
		if (params_size == 1)
			return sendToExecuter(RPL_UMODEIS(client->getNickname(), getMode(client)));
		std::string applied_modes = setModes(client);
		if (applied_modes.empty() || applied_modes.size() == 1)
			return;
		sendToExecuter(PRL_UMODECHANGE(client->getNickname(), applied_modes));
	}
}

std::string Mode::setModes(Channel *target) {

	bool	plus_flag;
	std::string	applied_modes;
	
	if (message.params[1][0] == '+')
		plus_flag = true;
	else if (message.params[1][0] == '-')
		plus_flag = false;
	else
		return NULL;
	applied_modes.append(1, message.params[1][0]);
	message.params[1].erase(0, 1); //これよくないね

	std::vector<std::string>::iterator	mode_params_it = message.params.begin();
	std::advance(mode_params_it, 2);

	for (size_t i = 0; i < message.params[1].size(); i++) {
		if (message.params[1][i] == 'i') {
			if (plus_flag)
				target->setMode(MODE_INVITE);
			else 
				target->unsetMode(MODE_INVITE);
			applied_modes.append("i");
		}
		else if (message.params[1][i] == 't') {
			if (plus_flag)
				target->setMode(MODE_TOPICLOCK);
			else
				target->unsetMode(MODE_TOPICLOCK);
			applied_modes.append("t");
		}
		else if (message.params[1][i] == 'k') {
			
			if (plus_flag) {
				if (target->isPasswordSet()) {
					sendToExecuter(ERR_KEYSET(target->getName()) + "\r\n");
					return applied_modes;
				}
				if (mode_params_it == message.params.end()) {
					sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), message.command) + "\r\n");
					return applied_modes;
				}
				target->setMode(MODE_KEY);
				target->setPassword(*mode_params_it);
				++ mode_params_it;
				applied_modes.append("k");
			}
			else{
				target->unsetMode(MODE_KEY);
				target->unsetPassword();
			}
		}
		else if (message.params[1][i] == 'l') {
			if (plus_flag) {
				if (mode_params_it == message.params.end()) {
					sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), message.command) + "\r\n");
					return applied_modes;
				}
				int limit = atoi_checked(*mode_params_it);
				if (limit < 1) {
					sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), message.command) + "\r\n");
					return applied_modes;
				}
				target->setMode(MODE_LIMIT);
				target->setLimit(limit); // validater追加
				++ mode_params_it;
				applied_modes.append("l");
			}
			else
				target->unsetMode(MODE_TOPICLOCK);

			applied_modes.append("l");
		}
		else if (message.params[1][i] == 'o') {
			if (plus_flag) {
				if (mode_params_it == message.params.end()) {
					sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), message.command) + "\r\n");
					return  applied_modes;
				}
				if (target->isMember(*mode_params_it)){
					sendToExecuter(ERR_USERNOTINCHANNEL(executer->getNickname(), target->getName()) + "\r\n");
					return applied_modes;
				}
				target->setMemberMode(*mode_params_it, 'o');
				++ mode_params_it;
				applied_modes.append("o");
			}
			else
				target->setMemberMode(*mode_params_it, 'v');
			
			applied_modes.append("o");
		}
		else {
			sendToExecuter(ERR_UNKNOWNMODE(message.params[1][i], target->getName()) + "\r\n");
			return applied_modes;
		}
	}
	return applied_modes;
}

std::string Mode::setModes(Client *target) {
	bool	plus_flag;
	std::string	applied_modes;
	
	if (message.params[1][0] == '+')
		plus_flag = true;
	else if (message.params[1][0] == '-')
		plus_flag = false;
	else
		return NULL;
	applied_modes.append(1, message.params[1][0]);
	message.params[1].erase(0, 1);

	std::vector<std::string>::iterator	mode_params_it = message.params.begin();
	std::advance(mode_params_it, 2);

	for (size_t i = 0; i < message.params[1].size(); i++) {
		if (message.params[1][i] == 'o') {
			if (!target->hasMode(USER_MODE_OPERATOR)){
					return applied_modes;
			}
			if (plus_flag) {
				if (mode_params_it == message.params.end()) {
					sendToExecuter(ERR_NEEDMOREPARAMS(executer->getNickname(), message.command) + "\r\n");
					return  applied_modes;
				}
				target->setMode(USER_MODE_OPERATOR);
				applied_modes.append("o");
			}
			else 
				target->unsetMode(USER_MODE_OPERATOR);
			applied_modes.append("o");
		}
		else {
			sendToExecuter(ERR_UMODEUNKNOWNFLAG + "\r\n");
			return applied_modes;
		}
	}
	return applied_modes;
}


std::string Mode::getMode( Client* client ) {
    std::string modeFlag;

    modeFlag.append( ( client->hasMode(USER_MODE_OPERATOR) ) ? "+o" : "-o" );
    return modeFlag;
}

std::string Mode::getMode( Channel* channel ) {
    std::string modeFlag;

    modeFlag.append( "+" );

    modeFlag.append( ( channel->hasMode(MODE_LIMIT) ? "l" : "" ));
    modeFlag.append( ( channel->hasMode(MODE_INVITE) ) ? "i" : "" );
    modeFlag.append( ( channel->hasMode(MODE_KEY) ) ? "k" : "" );
    modeFlag.append( ( channel->hasMode(MODE_TOPICLOCK)) ? "t" : "" );

    return modeFlag;
}

int atoi_checked(std::string literal) {
   long l;

	for (size_t i = 0 ; literal[i] != '\0'; i ++) {
		if (!std::isdigit(literal[i])) {
			return -1;
		}
	}

	l = atol(literal.c_str());
	if (l > INT_MAX)
		return INT_MAX;
	else
		return l;
}