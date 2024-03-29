/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/03/29 21:00:56 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"
#include "../../include/header.hpp"

void Servrr::trimSpaces(const std::string& str)
{
    std::stringstream splt(str);
    std::string sv;
    std::string s;
    splt >> args[0];
    splt >> args[1];
}

void Servrr::parsNick(clientito& client) {
    std::string str = "@&#:1234567890";
    if(str.find(args[1][0]) != std::string::npos)
        client.setnflag(false);
}

void	Servrr::auth2(char *str, clientito& client)
{
    trimSpaces(str);
    if(args[0] == "PASS"  && args[1] == _password && client.getpflag() == false) {
        client.setpflag(true);
        args[0].erase();
        args[1].erase();
        return ;
    }
    else if (client.getpflag() == false) {
        std::string buffer = "abelfany";
        std::string cmd = inet_ntoa(_addr.sin_addr);
        sendMsgToClient(client.getClinetFd(), ERR_PASSWDMISMATCH(buffer, cmd));
        return ;
    }
    if(args[0] == "NICK" && !args[1].empty() && client.getpflag() && !client.getnflag()) {
        args[0].erase();
        args[1].erase();
        client.setnflag(true);
        // parsNick(client);
        return ;
    }
    else if (client.getpflag() && !client.getnflag()) {
        sendMsgToClient(client.getClinetFd(), "Nickname ain't correct, try again ... (in a the form above)\n");
        return ;
    }
    if(args[0] == "USER" && !args[1].empty() && client.getpflag() && client.getnflag() && !client.getuflag()) {
        client.setuflag(true);
        args[0].erase();
        args[1].erase();
    }
    else if (!client.getuflag())
       sendMsgToClient(client.getClinetFd(), "Username ain't correct, try again ... (in a the form above)\n");
    if (client.getuflag()) {
        const char msg[179] = AUTHED;
        sendMsgToClient(client.getClinetFd(), msg);
    }
    return ;
}

