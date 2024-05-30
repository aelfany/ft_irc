/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:22:03 by abelfany          #+#   #+#            */
/*   Updated: 2024/04/06 02:23:40 by abelfany         ###  ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

//MODE 
#define SET_I       mod[ind].first == "+i"
#define REMOVE_I    mod[ind].first == "-i"
#define SET_T       mod[ind].first == "+t"
#define REMOVE_T    mod[ind].first == "-t"
#define SET_K       mod[ind].first == "+k"
#define REMOVE_K    mod[ind].first == "-k"
#define SET_O       mod[ind].first == "+o"
#define REMOVE_O    mod[ind].first == "-o"
#define SET_L       mod[ind].first == "+l"
#define REMOVE_L    mod[ind].first == "-l"

// nimeric

#define RPL_NONICKNAMEGIVEN(nick, host)                                     ":" + host + " 431 " + nick + " :No nickname given !\r\n"
#define RPL_ERRONEUSNICKNAME(nick, host)                                    ":" + host + " 432 " + nick + " :Erroneus nickname !\r\n"
#define RPL_NICKCHANGE(oldNick, nick, hostname)                             ":" + oldNick + " NICK " + nick + "\r\n"
#define NICKNAME_RPLY(nickname, username, hostname, newNickName)            ":" + nickname + "!~" + username + "@" + hostname + " NICK :" + newNickName  + "\r\n"

#define RPL_MODE(channel, nick, mode)                                      ":" + nick + " MODE " + channel + " " + mode + "\r\n"
#define RPL_NOTOPIC(hostname, nick, chann)                                  ":" + hostname + " 331 " + nick + " " + chann + " :No topic is set.\r\n"
#define RPL_TOPIC(hostname, nick, chann, topic)                             ":" + hostname + " 332 " + nick + " " + chann + " :" + topic + "\r\n"
#define ERR_INVITEONLY(hostname, nick, channelName)                        ":" + hostname + " 473 " + nick + " " + channelName + " :Cannot join channel, you must be invited (+i)\r\n"
// :*.freenode.net 333 dsa #chan ads!~fd@freenode-obu.d75.6g0qj4.IP :1716654989
#define RPL_TOPICWHOTIME(client, channel, nick, seter,username, setat)                      ":" + client + " 333 " + nick + " " + channel + " " + seter + "!~@oldschool_irc :" + setat + "\r\n"
// #define RPL_TOPICWHOTIME(client, channel, nick, setat)                      ":" + client + " 333 " + channel + " " + nick + "  " + setat + "\r\n"
#define RPL_TOPICDISPLAY(hostname, nick, channel, topic)                    ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_INVITING(hostname, inviting, invited, channel)                 ":" + hostname + " 341 " + inviting + " " + invited + " " + channel + " :Inviting " + invited + " to " + channel + "\r\n"
#define RPL_NEWTOPICSETTED(nick, username, hostname, channel, topic)        ":" + nick + "!~" + username + "@" + hostname + " TOPIC " + channel + " :" + topic + "\r\n"
#define RPL_INVITE(nick, username ,clienthostname, invited, channel)       ":" + nick + "!" + username + "@" + clienthostname + " INVITE " + invited + " :" + channel + "\r\n"
// ------- new 

#define RPL_MYINFO(nick, hostname)                                          ":" + hostname + " 004 " + nick + " :Host: " + hostname + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n"
#define RPL_WELCOME(nick, hostname)                                         ":" + hostname + " 001 " + nick + " :Welcome " + nick + " to the oldschool_irc network !\r\n"
#define RPL_YOURHOST(nick, hostname)                                        ":" + hostname + " 002 " + nick + " :Your host is " + hostname + " running version 1.0 !\r\n"
#define RPL_CREATED(nick, hostname, time)                                   ":" + hostname + " 003 " + nick + " :This server was created " + time + " !\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname)                         ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
#define RPL_JOIN(nick, username, channelname, ipaddress)                    ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
#define RPL_NAMREPLY(hostname, clients, channelname, nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"

#define ERR_NOTREGISTERED(buffer)                                           "451 " + buffer + " :You have not registered"
#define ERR_NICKNAMEINUSE(buffer)                                           "433 " + buffer + " " + buffer  + " :Nickname is already in use\r\n"
#define ERR_WASNOSUCHNICK(buffer,cmd)                                       ":" + buffer + " 406 " + cmd + " :There was no such nickname\r\n"
#define ERR_PASSWDMISMATCH(buffer, cmd)					                    (":" + cmd + " 464 " + buffer + " :Password is incorrect\r\n")
#define ERR_NEEDMOREPARAMS(client, cmd)             	                    ":" + client + " 461 " + cmd + " :Not enough parameters\r\n"
#define ERR_UNKNOWNCOMMAND(client, cmd)             	                    ":" + client + " 421 " + cmd + " :Unknown command\r\n"
#define ERR_NOTONCHANNEL(client, channel)                                   "442 " + client + " " + channel + " :You're not on that channel\r\n"
#define ERR_BADCHANNELKEY(buffer, channel)                                  "475 " + buffer + " " + channel + " :Cannot join channel (+k)\r\n"
#define ERR_CHANNELISFULL(buffer, channel)                                  "471 " + buffer + " " + channel + " :Cannot join channel (+l)"
#define ERR_ALREADYREGISTERED(buffer, nick)                                 (":" + buffer + " 462 " + nick + " :You may not reregister\r\n")
#define ERR_TOOMANYCHANNELS(buffer, channel)                                "405 " + buffer + " " + channel + " :You have joined too many channels"
#define ERR_CANNOTSENDTOCHAN(buffer, channel)                               "404 " + buffer + " " + channel + " :Cannot send to channel\r\n"
#define ERR_CHANOPRIVSNEEDED(buffer, channel)                               ":" + buffer + " 482 " + channel + " :You're not channel operator\r\n"
#define ERR_NOSUCHCHANNEL(buffer, nick, channel)                            ":" + buffer + " 403 " + nick + " " + channel + " :No such channel\r\n"

#define ERR_NOSUCHNICK(buffer, nickname)                                    "401 " + buffer + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(buffer, nickname, channel)                     "441 " + buffer + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_UNKNOWNMODE(host, nick, modechar)                                ":" + host + " 472 " + nick + " " + modechar + " :is unknown mode char to me\r\n"
// :*.freenode.net 472 fas V :is not a recognised channel mode.



/* command Responses */
#define RPL_NOTOP(nickname)                                                 (": 667 " + nickname + ": User not operator\r\n")
#define RPL_ALLOP(nickname)                                                 (": 666 " + nickname + ": User already operator\r\n")
#define RPL_ALLINV(nickname)                                                (": 665 " + nickname + ": Channel already set in invite-only mode\r\n")
#define RPL_NOTINV(nickname)                                                (": 664 " + nickname + ": Channel not in invite-only mode\r\n")
#define RPL_LIMITSET(nickname)                                              (": 662 " + nickname + ": This limit was set\r\n")
#define RPL_PASSNOTC(nickname)                                              (": 996 " + nickname + ": Password not correct\r\n")
#define RPL_NOPASSSET(nickname)                                             (": 997 " + nickname + ": No assword set\r\n")
#define RPL_PING(buffer, token)                                             ":" + buffer + " PONG :" + token
#define RPL_ALREADYSET(nickname)                                            (": 998 " + nickname + ": this Password already set\r\n")
#define RPL_NOLIMITSET(nickname)                                            (": 663 " + nickname + ": No limit set\r\n")
#define RPL_AUTHENTICATED(nickname)                                         (": 999 " + nickname + ": You are now logged in as " + nickname + "\r\n")
#define RPL_QUIT(nick, host, message)                                       ":" + nick + "!" + "~" + nick + "@" + host + " QUIT : " + message + "\r\n"
#define RPL_NOTICE(buffer, target, message)                                 ":" + buffer + " NOTICE " + target + " :" + message
#define RPL_PART(buffer,host,user, channel)                                 ":" + buffer + "!~" + user + "@" + host + " PART :" + channel + "\r\n" 
#define RPL_PRIVMSG(buffer, target, message)                                ":" + buffer + " PRIVMSG " + target + " :" + message
#define RPL_KICK(nick, username, hostname, kicked, channel, reason)         ":" + nick + "!" + "~" + username + "@" + hostname + " KICK " + channel + " " + kicked + " " + reason + "\r\n"
#define RPL_NOKEY(nickname,channel, mode)                                   ": 661 " + nickname + " :" + channel + " " + mode + " * You must specify a parameter for the key mode. Syntax: <key>.\r\n"


#endif//