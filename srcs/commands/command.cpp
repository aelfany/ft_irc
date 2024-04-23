/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/23 08:07:52 by idryab           ###   ########.fr       */
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
    return it->second;
}
void Servrr::command(std::string buffer, size_t i) {
    std::string s = inet_ntoa(_addr.sin_addr);
    std::string channel;
    std::string nick = getClientitoByIndex(i-1).getNickName();
    Channel a;
    trimSpaces(buffer,false);
    if(getClientitoByIndex(i-1).isAuthed() == false) {
        auth2(buffer, getClientitoByIndex(i-1));
    }
    else {
        if(args[0] == "PASS") {
            if(args.size() < 2)
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,"abelfany"));
            else
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_ALREADYREGISTERED(s,"abelfany"));
        }
        else if(args[0] == "JOIN") {
            trimSpaces(buffer,true);
            if(args.size() < 2)
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,"abelfany"));
            else
                createChannel(buffer, getClientitoByIndex(i-1).getClinetFd());
        }
        else if(args[0] == "TOPIC") {
        }
        else if(args[0] == "MODE")
        {
            try {
                Channel &mode = getChannel(args[1]);
                channel = getClientitoByIndex(i-1).getNickName();
                std::cout << "channel found secssusfly '*`" << std::endl;
                if(SET_I) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else
                        mode.setInvOnly(true);
                }
                else if(REMOVE_I) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else    
                        mode.setInvOnly(false);
                }
                if(SET_T) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else    
                        mode.setTopc(true);       
                }
                else if(REMOVE_T) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else    
                        mode.setTopc(false);
                }
                //**************************//
                //-------need parsing-------//
                if(SET_K) {
                    if(mode.getPass() == true)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ALREADYSET(s));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else {
                        std::cout << "33333" << std::endl;
                        mode.setPassword(args[3]);
                        mode.setPass(true);
                    }
                }
                else if(REMOVE_K) {
                    if(mode.getPass() == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOPASSSET(s));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else {
                            mode.getPassword().erase();
                            mode.setPass(false);
                    }
                }
                //---------------------//
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
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(s, nick));
            }
        }
        // std::string _message;
        // size_t pos = _recvBuffer.find(":");
        // _message = _recvBuffer.substr(pos, std::string::npos);
        else if (args[0] == "PRIVMSG")
        {
            std::string _message;
            for(size_t i = 0; i < args.size(); i++)
            {
                if(i > 1)
                {
                    if (args[i][0] == ':')
                        _message = args[i].substr(1, std::string::npos) + " ";
                    else
                        _message += args[i] + " ";
                }
            }
            sendmessage(getClientitoByIndex(i-1), args[1], _message);
        }
    }
    args.clear();
}
            // puts("******************************");
            // std::cout << mode.getChannelName() << std::endl; 
            // std::cout << mode.getPassword() << std::endl; 
            // puts("******************************");