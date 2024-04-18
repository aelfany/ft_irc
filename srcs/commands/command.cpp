/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/18 06:11:40 by idryab           ###   ########.fr       */
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
    std::string nick = getClientitoByIndex(i-1).getNickName();
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
        else if(args[0] == "MODE")
        {
            try {
                //****** no parsing ******//
                Channel &mode = getChannel(args[1]);
                std::cout << "name: " << mode.getChannelName() << std::endl;
                std::cout << "channel found secssusfly '*`" << std::endl;
                if(SET_I) {
                    if (getClientitoByIndex(i-1).getOperator() == true)
                        mode.setInvOnly(true);
                }
                else if(REMOVE_I) {
                    if (getClientitoByIndex(i-1).getOperator() == true)
                        mode.setInvOnly(false);
                }
                if(SET_T) {
                    if (getClientitoByIndex(i-1).getOperator() == true)
                        mode.setTopc(true);       
                }
                else if(REMOVE_T) {
                    if (getClientitoByIndex(i-1).getOperator() == true)
                        mode.setTopc(false);
                }
                //**************************//
                //-------need parsing-------//
                if(SET_K) {
                    if(mode.getPass() == true)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ALREADYSET(s));
                    else
                    {
                        if (getClientitoByIndex(i-1).getOperator() == true)
                        {
                            mode.setPassword(args[3]);
                            mode.setPass(true);
                        }
                    }
                }
                else if(REMOVE_K) {
                    if(mode.getPass() == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOPASSSET(s));
                    else
                    {
                        if (getClientitoByIndex(i-1).getOperator() == true)
                        {
                            mode.getPassword().erase();
                            mode.setPass(false);
                        }
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
        // else 
    }
    args.clear();
}
            // puts("******************************");
            // std::cout << mode.getChannelName() << std::endl; 
            // std::cout << mode.getPassword() << std::endl; 
            // puts("******************************");
