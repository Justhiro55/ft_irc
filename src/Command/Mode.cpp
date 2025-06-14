#include "Command.hpp"

Mode::Mode() :AbstractCommand() {}

Mode::~Mode() {}

void Mode::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return ; // 484  ERR_RESTRICTED ":Your connection is restricted!

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size < 1)
		return ;
    if ( params.front()[0] == '#' || params.front()[0] == '&' ) {
        if ( params.front().size() <= 1 )
            return ;// error

        Channel* channel;
		std::string channel_name = params.front().erase(0, 1);
        channel = serverData->getChannelByName(channel_name);
		if (channel == NULL)
			return ; //eっろr
		if (channel->isOperatorMember(executer->getNickname()))
			return ; //
		
		if (params_size == 1) {
			sendToExecuter("/r/n"); //RPL_CHANNELMODEIS； 324 Alice #chan getMode(executer) 
			return ;
		}

		setModes()
			
		
    }
}

std::string Mode::setModes(Channel *target) {

	bool	plus_flag;
	ssize_t params_size = message.params.size();
	std::string	applied_modes;
	
	if (message.params[1][0] == '+')
		plus_flag = true;
	else if (message.params[1][0] == '-')
		plus_flag = false;
	else
		return NULL;
	applied_modes.append(1, message.params[1][0]);
	message.params[1].erase(0, 1);

	ssize_t mode_params_size = params_size - 2;
	int		mode_params_index = 0;
	for (int i = 0; message.params[1][i] != '/0'; i++) {
		if (message.params[1][i] == 'i') {
			target->setMode(MODE_INVITE);
			applied_modes.append("i");
		}
		else if (message.params[1][i] == 't') {
			target->setMode(MODE_TOPICLOCK);

		}
		else if (message.params[1][i] == 'k') {
			target->setMode(MODE_KEY);
			target->setPassword(message.params[mode_params_index]);
			mode_params_index ++;
			applied_modes.append("k");
		}
		else if (message.params[1][i] == 'l') {
			target->setMode(MODE_LIMIT);

			mode_params_index ++;
			applied_modes.append("l");
		}
		else if (message.params[1][i] == 'l') {
			
		
		else {

		}
	}
}

std::string Mode::setModes(Client *target) {

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
    modeFlag.append( ( channel->hasMode(MODE_OPERATOR) ) ? "o" : "" );

    return modeFlag;
}