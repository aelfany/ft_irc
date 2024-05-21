/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 02:24:03 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/21 16:03:53 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"
#include "../../include/replies.hpp"

void Channel::deletClient(std::string nick) {
    map_users::iterator it = _users.begin();
    
    std::cout << "\033[0;31m { " << _users.size() << " }\033[0m" << std::endl;
    while (it != _users.end()) {
        if (it->first.second == nick) {
            std::cout << "\033[0;31m { " << it->first.second << " }\033[0m" << std::endl;
            _users.erase(it++);
        } else {
            ++it;
        }
    }
    std::cout << "\033[0;31m { " << _users.size() << " }\033[0m" << std::endl;
}
void Servrr::split(std::string buffer, size_t i) {
    std::stringstream users(buffer);
    std::string user;
    std::string nick = getClientitoByIndex(i-1).getNickName();
    while (std::getline(users,user, ',')) {
    try {
        Channel &mode = getChannel(args[1]);
        if(mode.checkUserexist(nick) == false)
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(host,nick));
        else if(mode.getPrvBynickname(nick) == false)
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,args[1]));
        else if(mode.checkUserexist(user) == false)
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(host,args[1]));
        else {
            std::cout << "\033[0;31m { " << user << " }\033[0m" << std::endl;
            SendToAll(mode, RPL_KICK(nick,getClientitoByIndex(i-1).getUserName(),host,args[2],args[1], "fuck"));
            mode.deletClient(args[2]);
        }
    }
    catch(...) {
        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, args[1]));
    }
    }
    
}

void Servrr::kick(std::string buffer, size_t i) {
    split(buffer, i);
}