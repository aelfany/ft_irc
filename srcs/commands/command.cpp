/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/16 15:49:55 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"


Channel & Servrr::getChannel(std::string channel) {

    if(args[1][0] == '#')
        args[1] = args[1].substr(1, std::string::npos);
    std::map<std::string, Channel>::iterator it = _channels.find(channel);
    if (it == _channels.end())
        throw "No channel found";
    // puts("******************************");
    // std::cout << it->second.getChannelName() << it->second.getPassword() << std::endl; 
    // puts("******************************");
    return it->second;
}
void Servrr::command(std::string buffer, size_t i, Servrr& servrr) {
    std::string s = inet_ntoa(_addr.sin_addr);
    std::string nick = servrr.getClientito(i-1).getNickName();
    trimSpaces(buffer,false);
    std::cout << "->>>" << args.size() << std::endl;
    if(servrr.getClientito(i-1).isAuthed() == false) {
        servrr.auth2(buffer, servrr.getClientito(i-1));
    }
    else {
        if(args[0] == "PASS") {
            if(args.size() < 2)
                sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,"abelfany"));
            else
                sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), ERR_ALREADYREGISTERED(s,"abelfany"));
        }
        else if(args[0] == "JOIN") {
            trimSpaces(buffer,true);
            std::cout << "->>>" << args.size() << std::endl;
            if(args.size() < 2)
                sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,"abelfany"));
            else
                servrr.createChannel(buffer, servrr.getClientito(i-1).getClinetFd());
        }
        else if(args[0] == "MODE")
        {
            try {
                Channel &mode = getChannel(args[1]); 
                std::cout << "channel found secssusfly '*`" << std::endl;
                if(SET_I) {
                    mode.setInvOnly(true);
                }
                else if(REMOVE_I) {
                    mode.setInvOnly(false);
                }
                if(SET_T) {
                    mode.setTopc(true);       
                }
                else if(REMOVE_T) {
                    mode.setTopc(false);
                }
                if(SET_K) {
                    if(mode.getPass() == true)
                        sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), RPL_ALREADYSET(s));
                    else
                    {
                       mode.setPassword(args[3]);
                       mode.setPass(true);
                    }
                }
                else if(REMOVE_K) {
                    if(mode.getPass() == false)
                        sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), RPL_NOPASSSET(s));
                    else
                        mode.getPassword().erase();
                }
                if(SET_O) {
                    /***/
                }
                else if(REMOVE_O) {
                    /***/
                }
                if(SET_L) {
                    std::stringstream ss(args[3]);
                    size_t limit;
                    ss >> limit;
                    mode.setLimit(limit);
                    mode.setUserLimit(true);
                }
                else if(REMOVE_L) {
                    mode.setUserLimit(false);
                    mode.setLimit(-1);
                }
            }
            catch(...) {
                sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(s, nick));
            }
        }
        // else 
    }
    args.clear();
}
            // puts("******************************");
            // std::cout << mode.getChannelName() << std::endl; 
            // std::cout << mode.getPassword() << std::endl; 
            // puts("******************************");
