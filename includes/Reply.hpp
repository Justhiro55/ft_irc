#pragma once

// Welcome
#define RPL_WELCOME(nick, user, host) "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host
#define RPL_YOURHOST(nick, servername, version) "002 " + nick + " :Your host is " + servername + ", running version " + version
#define RPL_CREATED(nick, date) "003 " + nick + " :This server was created " + date
#define RPL_MYINFO(nick, servername, version, usermodes, channelmodes) "004 " + nick + " " + servername + " " + version + " " + usermodes + " " + channelmodes

// PASS
#define ERR_PASSWDMISMATCH(nick) "464 " + nick + " :Password incorrect"

// NICK
#define ERR_NONICKNAMEGIVEN(nick) "431 " + nick + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick, badnick) "432 " + nick + " " + badnick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nick, badnick) "433 " + nick + " " + badnick + " :Nickname is already in use"
#define ERR_NICKCOLLISION(nick, badnick, user, host) "436 " + nick + " " + badnick + " :Nickname collision KILL from " + user + "@" + host
#define ERR_UNAVAILRESOURCE(nick, resource) "437 " + nick + " " + resource + " :Nick/channel is temporarily unavailable"

// JOIN
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY(nick, channel) "475 " + nick + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANNELISFULL(channel) "471 " + channel + ":Cannot join channel (+l)"

// INVITE
#define ERR_USERONCHANNEL(nick, target, channel) "443 " + nick + " " + target + " " + channel + " :is already on channel"
#define RPL_INVITING(nick, channel, target) "341 " + nick + " " + channel + " " + target

// KICK
#define ERR_BADCHANMASK(nick, channel) "476 " + nick + " " + channel + " :Bad Channel Mask"

// Oper
#define RPL_YOUREOPER(nick)   std::string ("381 ") + nick + ":You are now an IRC operator"

//MODE channle
#define ERR_UNKNOWNMODE(mode, channel) std::string("472 ") + mode + " :is unknown mode char to me for " + channel
#define ERR_KEYSET(channel) "467 " +  channel + " :Channel key already set"
#define RPL_CHANNELMODEIS(nick, channel, modes) "324 " + nick + " " + channel + " :" + modes
#define RPL_CHANNELMODECHANGE(nick, user, host, channel, mode_changes) ":" + nick + "!" + user + "@" + host + " MODE " + channel + " :" + mode_changes

//MODE user
#define ERR_USERSDONTMATCH(client) "502 " + client + " :Cannot change mode for other users"
#define ERR_UMODEUNKNOWNFLAG(nick) std::string("501 ") + nick + " :Unknown MODE flag"
#define	RPL_UMODEIS(nick, modes) "221 " + nick + " :" + modes  
#define	PRL_UMODECHANGE(nick, modes) "MODE " + nick + " :" + modes 

// Topic
#define RPL_NOTOPIC(channel)  "331 " + channel + ":No topic is set"
#define RPL_TOPIC(nick, channel, topic)  "332 " + nick + " "+ channel + " :" + topic
#define ERR_NOCHANMODES(channel)  "447 " +  channel + ":Channel doesn't support modes"

#define MSG_TOPIC(nick, user, host, channel, topic) \
    std::string(":") + nick + "!" + user + "@" + host + " TOPIC " + channel + " :" + topic + "\r\n"

// Part
#define MSG_PART_DEFAULT(nick, user, host, channel) \
    (std::string(":") + (nick) + "!" + (user) + "@" + (host) + \
    " PART " + (channel) + " :" + (nick) + "\r\n")
#define MSG_PART_REASON(nick, user, host, channel, msg) \
    (std::string(":") + (nick) + "!" + (user) + "@" + (host) + \
    " PART " + (channel) + " :" + (msg) + "\r\n")

// Privmsg/NOTICE
#define ERR_NORECIPIENT(command) std::string("411 ") + ":No recipient given " + command
#define ERR_NOTEXTTOSEND(nick)   std::string("412 ") + nick + " :No text to send"
#define ERR_TOOMANYTARGETS(nick, target) std::string("403 ") + nick + " " + target + " :Duplicate recipients. No message delivered"

#define MSG_PRIVMSG(sender_nick, sender_user, sender_host, channel, message) \
    std::string(":") + sender_nick + sender_user + sender_host + \
    " PRIVMSG " + channel + " :" + message + "\r\n"
#define MSG_NOTICE(sender_nick, sender_user, sender_host, channel, message) \
    std::string(":") + sender_nick + sender_user + sender_host + \
    " NOTICE " + channel + " :" + message + "\r\n"

// Common errors
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(nick, channel) "404 " + nick + " " + channel + " :Cannot send to channel"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel"
#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTRED(nick) "462 " + nick + " :Unauthorized command (already registered)"
#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered"
#define ERR_NOSUCHNICK(nick) "401 " + nick + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(nick, channel) "441 " + nick + " " + channel + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " + nick + " " + channel + " :You're not channel operator"
#define ERR_RESTRICTED(executer) "484 " + executer + " :Your connection is restricted!"
