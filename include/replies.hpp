/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:22:03 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/06 02:23:40 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

//MODE 
#define SET_I       args[2] == "+i"
#define REMOVE_I    args[2] == "-i"
#define SET_T       args[2] == "+t"
#define REMOVE_T    args[2] == "-t"
#define SET_K       args[2] == "+k"
#define REMOVE_K    args[2] == "-k"
#define SET_O       args[2] == "+o"
#define REMOVE_O    args[2] == "-o"
#define SET_L       args[2] == "+l"
#define REMOVE_L    args[2] == "-l"

// nimeric

#define ERR_NOTREGISTERED(buffer)                       "451 " + buffer + " :You have not registered"
#define ERR_ALREADYREGISTERED(buffer, nick)             (":" + buffer + " 462 " + nick + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(buffer, cmd)					(":" + cmd + " 464 " + buffer + " :Password is incorrect\r\n")
#define ERR_NEEDMOREPARAMS(buffer, cmd)             	":" + buffer + " 461 " + cmd + " :Not enough parameters\r\n"
#define ERR_NONICKNAMEGIVEN(buffer)                     "431 " + buffer + " :Nickname not given"
#define ERR_NICKNAMEINUSE(buffer)                       "433 " + buffer + " " + buffer  + " :Nickname is already in use\r\n"

#define ERR_UNKNOWNcmd(buffer, cmd)             		"421 " + buffer + " " + cmd + " :Unknown cmd"

#define ERR_TOOMANYCHANNELS(buffer, channel)            "405 " + buffer + " " + channel + " :You have joined too many channels"
#define ERR_NOTONCHANNEL(buffer, channel)               "442 " + buffer + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(buffer, channel)              "403 " + buffer + " " + channel + " :No such channel"
#define ERR_BADCHANNELKEY(buffer, channel)              "475 " + buffer + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANNELISFULL(buffer, channel)              "471 " + buffer + " " + channel + " :Cannot join channel (+l)"
#define ERR_CANNOTSENDTOCHAN(buffer, channel)           "404 " + buffer + " " + channel + " :Cannot send to channel"
#define ERR_CHANOPRIVSNEEDED(buffer, channel)           ":" + buffer + " 482 " + channel + " :You're not channel operator\r\n"

#define ERR_NOSUCHNICK(buffer, nickname)                "401 " + buffer + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(buffer, nickname, channel) "441 " + buffer + " " + nickname + " " + channel + " :They aren't on that channel"

/* Numeric Responses */

#define RPL_WELCOME(buffer, netw)                       (":" + buffer + " 001 " + netw + " :Welcome to the ft_irc network\r\n")
#define RPL_NAMREPLY(buffer, channel, users)            ":" + buffer + " 353 " + buffer + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(buffer, channel)                 "366 " + buffer + " " + channel + " :End of /NAMES list."


/* command Responses */
// #define RPL_JOIN(buffer, channel)                       ":" + buffer + " JOIN :" + channel
#define RPL_PART(buffer, channel)                       ":" + buffer + " PART :" + channel
#define RPL_PING(buffer, token)                         ":" + buffer + " PONG :" + token
#define RPL_PRIVMSG(buffer, target, message)            ":" + buffer + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(buffer, target, message)             ":" + buffer + " NOTICE " + target + " :" + message
#define RPL_QUIT(buffer, message)                       ":" + buffer + " QUIT :Quit: " + message
#define RPL_KICK(buffer, channel, target, reason)       ":" + buffer + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(buffer, channel, modes, args)          ":" + buffer + " MODE " + channel + " " + modes + " " + args
#define RPL_AUTHENTICATED(nickname)                     (": 999 " + nickname + ": You are now logged in as " + nickname + "\r\n")
#define RPL_ALREADYSET(nickname)                        (": 998 " + nickname + ": Password already set\r\n")
#define RPL_NOPASSSET(nickname)                        (": 997 " + nickname + ": No assword set\r\n")


#endif//