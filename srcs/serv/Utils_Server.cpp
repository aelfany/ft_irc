/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/04 02:58:15 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"
#include "../../include/header.hpp"

void Servrr::trimSpaces(const std::string& str)
{
    std::stringstream splt(str);
    std::string sv;
    while(splt >> sv)
        args.push_back(sv);
}
// void Servrr::trimUser(const std::string& str)
// {
//     std::stringstream splt(str);
//     std::string sv;
//     std::string s;
//     splt >> args[0]; splt >> args[1]; splt >> args[2]; splt >> args[3];
// }

void Servrr::parsNick(clientito& client) {
    std::string str = "@&#:1234567890";
    for(size_t a = 0; a < _clients.size(); a++) {
        if(args[1] == _clients[a].getNickName()) {
            client.setnflag(false);
            sendMsgToClient(client.getClinetFd(), ERR_NICKNAMEINUSE(args[1]));
        }
    }
    if(str.find(args[1][0]) != std::string::npos)
        client.setnflag(false);
}

// void Servrr::parsUser(clientito& client) {
// }
void	Servrr::auth2(std::string str, clientito& client)
{
    trimSpaces(str);
    std::cout << "----------------------" << std::endl;
    std::cout << args[0] << " " << args[1] << std::endl;
    std::cout << "----------------------" << std::endl;
    if(args[0] == "PASS"  && args[1] == _password && client.getpflag() == false) {
        client.setpflag(true);
        args.clear();
        return ;
    }
    else if (client.getpflag() == false) {
        std::string buffer = "abelfany";
        sendMsgToClient(client.getClinetFd(), ERR_PASSWDMISMATCH(inet_ntoa(_addr.sin_addr), buffer));
        args.clear();
        return ;
    }
    if(args[0] == "NICK" && !args[1].empty() && client.getpflag() && !client.getnflag()) {
        client.setnflag(true);
        std::cout << args[1] << std::endl;
        parsNick(client);
        if(client.getnflag())
            client.setNickName(args[1]);
        args.clear();
        return ;
    }
    else if (client.getpflag() && !client.getnflag()) {
        sendMsgToClient(client.getClinetFd(), "Nickname ain't correct, try again ... (in a the form above)\n");
        args.clear();
        return ;
    }
    if(args[0] == "USER" && !args[1].empty() && client.getpflag() && client.getnflag() && !client.getuflag()) {
        client.setuflag(true);
        args.clear();
    }
    else if (!client.getuflag())
       sendMsgToClient(client.getClinetFd(), "Username ain't correct, try again ... (in a the form above)\n");
    if (client.getuflag()) {
        const char msg[179] = AUTHED;
        sendMsgToClient(client.getClinetFd(), msg);
    }
    return ;
}

