/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/04 03:34:34 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"
#include "../../include/header.hpp"


void Servrr::command(std::string buffer, size_t i, Servrr& servrr) {
    trimSpaces(buffer);
    if(servrr.getClientito(i-1).isAuthed() == false) {
        std::string s1 = "test";
        std::string s2 = "test";
        sendMsgToClient(servrr.getClientito(i-1).getClinetFd(), RPL_WELCOME(s1, s2));
        servrr.auth2(buffer, servrr.getClientito(i-1));
    }
    else {
        if(args[0] == "JOIN" ) {
            servrr.createChannel(buffer, servrr.getClientito(i-1).getClinetFd());
        }
        if(args[0] == "MODE" ) {
        }
    }   
}
