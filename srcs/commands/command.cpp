/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/06 21:41:35 by idryab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"


void Servrr::command(std::string buffer, size_t i, Servrr& servrr) {
    std::string s = inet_ntoa(_addr.sin_addr);
    trimSpaces(buffer,false);
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
            if(args.size() < 2)
                sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,"abelfany"));
            else
                servrr.createChannel(buffer, servrr.getClientito(i-1).getClinetFd());
        }
        else if(args[0] == "MODE" ) {
            if(SET_I) {
                /***/
            }
            else if(REMOVE_I) {
                /***/
            }
            if(SET_T) {
                    
            }
            else if(REMOVE_T) {
                /***/
            }
            if(SET_K) {
                    
            }
            else if(REMOVE_K) {
                /***/
            }
            if(SET_O) {
                
            }
            else if(REMOVE_O) {
                /***/
            }
            if(SET_L) {
                    
            }
            else if(REMOVE_L) {
                /***/
            }
        }
        // else 
    }
    args.clear();
}
