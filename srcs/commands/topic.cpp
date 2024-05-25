/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:10:19 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/25 19:41:54 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/Server.hpp"
#include "../../include/client.hpp"

std::string  Servrr::_time(std::string nick) {
    (void)nick;
    time_t now = time(0);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%s", std::localtime(&now));
    return std::string(buffer);
}
void    Channel::setTopicAttr(std::string topic, bool topc, std::string seter, std::string time) {
    topicseter = seter;
    topictime = time;
    _topic = topic;
    _topc = topc;
}
void Servrr::Topic(std::string nick, size_t i) {
    try {
        Channel &mode = getChannel(args[1]);
        // if(mode.checkUserexist(nick) == false) {
        //         sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(host,nick));
        // }
        if(args.size() == 2) {
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(),RPL_TOPICDISPLAY(host, nick,mode.getChannelName(),mode.getTopic()));
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(),RPL_TOPICWHOTIME(host, mode.getChannelName(), nick,mode.gettopicseter(),getClientitoByIndex(i-1).getUserName(),mode.gettopictime()));
        }
        if(args.size() == 3) {
                mode.setTopicAttr(args[2], true, nick, _time(nick));
                if (mode.getPrvBynickname(nick) == false && mode.getTopc() == true && mode.checkUserexist(nick) == true) {
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,nick));      
                }
                SendToAll(mode, RPL_NEWTOPICSETTED(nick, mode.gettopicseter(), host,mode.getChannelName(),args[2]));
            }
        }
    catch(const char *str) {
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, args[1]));
    }
}