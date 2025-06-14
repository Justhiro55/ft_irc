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
#define ERR_RESTRICTED(executer) "484 " + executer + " :Your connection is restricted!"

// Common errors
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTRED(nick) "462 " + nick + " :Unauthorized command (already registered)"
#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered"
