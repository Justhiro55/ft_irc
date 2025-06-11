#include "Command.hpp"

Mode::Mode() :AbstractCommand() {}

Mode::~Mode() {}

void Mode::executeCmd() {
	if (!this->executer->getAuth() || !this->executer->getRegister())
		return ; // 484  ERR_RESTRICTED ":Your connection is restricted!

	std::vector<std::string> params = this->message.params;
	size_t params_size = params.size();

	if (params_size < 1 || params_size > 2)
		return ;
    if ( params.front()[0] == '#' || params.front()[0] == '&' ) {
        if ( params.front().size() <= 1 )
            return ;// error

        Channel* chan;
        chan = serverData->getChannelByName( params.front() );
		if (chan == NULL)
			return ; //error
        // if ( params.size() == 1 )
        //     return sendReply( RPL_CHANNELMODEIS, "", executer->getNickname(),  chan->getName() + " " +getMode( chan ) );

        // if ( params.size() >= 2 )
        //     setChanMode( params, client, chan );
        // else
        //     sendReply( ERR_UMODEUNKNOWNFLAG );
        // return ;
    }
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