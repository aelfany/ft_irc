/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:10:19 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/20 12:46:33 by abelfany         ###   ########.fr       */
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
    try {
        Channel &mode = getChannel(args[1]);
        if(mode.checkUserexist(nick) == false) {
                std::cout << "" << "{" << nick << "}" << std::endl;
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(host,nick));
        }
        // std::cout << "_(" << args.size() << ")_" << std::endl;
        if(args.size() == 2) {
            // std::cout << "" << "{" << mode.getTopic() << "}" << std::endl;
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(),RPL_TOPICDISPLAY(host, nick,mode.getChannelName(),mode.getTopic()));
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(),RPL_TOPICWHOTIME(host, nick, mode.getChannelName(),mode.gettopictime()));
        }
        if(args.size() == 3) {
                if (mode.getPrvBynickname(nick) == false && mode.getTopc() == true && mode.checkUserexist(nick) == true) {
                    // std::cout << "" << "{" << mode.getPrvBynickname(nick) << "}" << std::endl;
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,nick));      
                }
                mode.setTopicAttr(args[2], true, nick, _time(nick));
                SendToAll(mode, RPL_NEWTOPICSETTED(nick, mode.gettopicseter(), host,mode.getChannelName(),args[2]));
            }
            // mode.setTopic(args[2]);
            // sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(),RPL_NEWTOPICSETTED(nick, mode.gettopicseter(), s,mode.getChannelName(),args[2]));
        }
    catch(const char *str) {
            // printf("\033[0;31m //////////////////////////////////////////////// \033[0m\n");   
            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, "#" + args[1]));
    }
        
    // if(args.size() == 3)
}



// PART #A Leaving... if some one leaving 