/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 02:24:03 by abelfany          #+#    #+#             */
/*   Updated: 2024/06/03 10:21:42 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"
#include "../../include/replies.hpp"

void Channel::deletClient(std::string nick) {
    map_users::iterator it = _users.begin();
    while (it != _users.end()) {
        if (it->second.getNickName() == nick) {
            _users.erase(it++);
        }
        else
            ++it;
    }
}

void Servrr::split(std::string buffer, size_t i) {
    std::stringstream users(buffer);
    std::string user;
    std::string res;
    std::string nick = getClientitoByIndex(i-1).getNickName();
    while (std::getline(users,user, ',')) {
    try {
        Channel &mode = getChannel(args[1]);
        if(args.size() == 3 || (args.size() == 4 && args[3] == ":"))
            res = ":Break the Rules";
        else {
            for(size_t a = 3; a < args.size(); a++) {
                res += args[a];
                if(a < args.size()) 
                    res += " ";
            }
        }
        if(args.size() < 3)
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,"kick"));
        else if(mode.checkUserexist(nick) == false)
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(host,nick));
        else if(mode.getPrvBynickname(nick) == false)
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,args[1]));
        else if(mode.checkUserexist(user) == false)
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(host,args[1]));
        else {
            std::cout << res << std::endl;
            SendToAll(mode, RPL_KICK(nick,getClientitoByIndex(i-1).getUserName(),host,user,args[1], res));
            mode.deletClient(user);
            mode.setusersSize(-1);
        }
    }
    catch(const char *) {
        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, args[1]));
    }
    }
    
}

void Servrr::kick(std::string buffer, size_t i) {
    split(buffer, i);
}