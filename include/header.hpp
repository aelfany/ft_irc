/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:22:03 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/04 03:32:50 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include "client.hpp"
#include <map>

// nimeric

#define ERR_NOTREGISTERED(buffer)                       "451 " + buffer + " :You have not registered"
#define ERR_ALREADYREGISTERED(buffer, cmd)				(":" + cmd + " 462 " + buffer + " :You may not register\r\n")
#define ERR_PASSWDMISMATCH(buffer, cmd)					(":" + cmd + " 464 " + buffer + " :Password is incorrect\r\n")
#define ERR_NICKNAMEINUSE(buffer)                       (": 433 " + buffer + ":Nickname is already in use\r\n")
#define ERR_NONICKNAMEGIVEN(buffer)                     "431 " + buffer + " :Nickname not given"

#define ERR_UNKNOWNcmd(buffer, cmd)             		"421 " + buffer + " " + cmd + " :Unknown cmd"
#define ERR_NEEDMOREPARAMS(buffer, cmd)             	"461 " + buffer + " " + cmd + " :Not enough parameters"

#define ERR_TOOMANYCHANNELS(buffer, channel)            "405 " + buffer + " " + channel + " :You have joined too many channels"
#define ERR_NOTONCHANNEL(buffer, channel)               "442 " + buffer + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(buffer, channel)              "403 " + buffer + " " + channel + " :No such channel"
#define ERR_BADCHANNELKEY(buffer, channel)              "475 " + buffer + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANNELISFULL(buffer, channel)              "471 " + buffer + " " + channel + " :Cannot join channel (+l)"
#define ERR_CANNOTSENDTOCHAN(buffer, channel)           "404 " + buffer + " " + channel + " :Cannot send to channel"
#define ERR_CHANOPRIVSNEEDED(buffer, channel)           "482 " + buffer + " " + channel + " :You're not channel operator"

#define ERR_NOSUCHNICK(buffer, nickname)                "401 " + buffer + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(buffer, nickname, channel) "441 " + buffer + " " + nickname + " " + channel + " :They aren't on that channel"


/* Numeric Responses */

#define RPL_WELCOME(buffer, nick)                       ":" + buffer + "001 " + " :Welcome " + nick + " to the ft_irc network" + nick + "\r\n"
#define RPL_NAMREPLY(buffer, channel, users)            "353 " + buffer + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(buffer, channel)                 "366 " + buffer + " " + channel + " :End of /NAMES list."


/* command Responses */

#define RPL_JOIN(buffer, channel)                       ":" + buffer + " JOIN :" + channel
#define RPL_PART(buffer, channel)                       ":" + buffer + " PART :" + channel
#define RPL_PING(buffer, token)                         ":" + buffer + " PONG :" + token
#define RPL_PRIVMSG(buffer, target, message)            ":" + buffer + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(buffer, target, message)             ":" + buffer + " NOTICE " + target + " :" + message
#define RPL_QUIT(buffer, message)                       ":" + buffer + " QUIT :Quit: " + message
#define RPL_KICK(buffer, channel, target, reason)       ":" + buffer + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(buffer, channel, modes, args)          ":" + buffer + " MODE " + channel + " " + modes + " " + args

class channel {

    private:
        std::map<clientito, std::string> channel;//
    public:
        void setmap(clientito &obj, std::string nickname);
      
};

#endif