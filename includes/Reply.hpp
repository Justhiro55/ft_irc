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

// INVITE
#define ERR_USERONCHANNEL(nick, target, channel) "443 " + nick + " " + target + " " + channel + " :is already on channel"
#define RPL_INVITING(nick, channel, target) "341 " + nick + " " + channel + " " + target

// KICK
#define ERR_USERNOTINCHANNEL(nick, target, channel) "441 " + nick + " " + target + " " + channel + " :They aren't on that channel"
#define ERR_BADCHANMASK(nick, channel) "476 " + nick + " " + channel + " :Bad Channel Mask"

// Common errors
#define ERR_NOSUCHNICK(nick, target) "401 " + nick + " " + target + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(nick, channel) "404 " + nick + " " + channel + " :Cannot send to channel"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel"
#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTRED(nick) "462 " + nick + " :Unauthorized command (already registered)"
#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " + nick + " " + channel + " :You're not channel operator"
#define ERR_RESTRICTED(executer) "484 " + executer + " :Your connection is restricted!"
