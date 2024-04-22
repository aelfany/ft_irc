/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:10:19 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/22 16:23:12 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"

//Sun Apr 21 17:43:10 2024
// TTT set the topic at: Apr 21, 2024 at 6:12 PM

std::string  Servrr::_time(std::string nick) {
    time_t now = time(0);
    std::string tmp = ctime(&now);
    std::string time;
    std::stringstream s(tmp);
    std::string arr[5];
    for(int a = 0; s >> arr[a]; a++) {}
    time += nick + arr[1] + " " + arr[2] + ", " + arr[4] + " at " + arr[3];
    std::cout << time << std::endl;
    return time;
}
void    Channel::setTopicAttr(std::string topic, bool topc, std::string seter, std::string time) {
    topicseter = seter;
    topictime = time;
    _topic = topic;
    _topc = topc;
}
void Servrr::Topic(std::string nick, size_t i) {
    std::string s = inet_ntoa(_addr.sin_addr);
    // topictime = _time(nick + " " + "set the topic at: ");
    try {
        Channel &mode = getChannel(args[1]);
        if(args.size() < 3) {
                if(mode.checkUserexist(nick) == false)
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(s,nick));
                else if (mode.getPrvBynickname(nick) == false)
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,nick));      
            }
        }
    catch(const char *str) {
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(s, nick, "#" + args[1]));
    }
        
    // if(args.size() == 3)
}