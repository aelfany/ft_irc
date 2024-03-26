/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/03/26 10:23:41 by idryab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

void Servrr::trimSpaces(const std::string& str)
{
    std::stringstream splt(str);
    std::string sv;
    std::string s;
    splt >> args[0];
    splt >> args[1];
}

void	Servrr::auth2(char *str, clientito& client)
{
    trimSpaces(str);
    if(args[0] == "PASS"  && args[1] == _password && client.getpflag() == false)
    {
        client.setpflag();
        args[0].erase();
        args[1].erase();
        return ;
    }
    else if (client.getpflag() == false)
    {
        sendMsgToClient(client.getClinetFd(), "Password ain't correct, try again ... (in a the form above)\n");
        return ;
    }
    if(args[0] == "NICK" && !args[1].empty() && client.getpflag() && !client.getnflag())
    {
        client.setnflag();   
        args[0].erase();
        args[1].erase();
        return ;
    }
    else if (client.getpflag() && !client.getnflag())
    {
        sendMsgToClient(client.getClinetFd(), "Nickname ain't correct, try again ... (in a the form above)\n");
        return ;
    }
    if(args[0] == "USER" && !args[1].empty() && client.getpflag() && client.getnflag() && !client.getuflag())
    {
        client.setuflag();
        args[0].erase();
        args[1].erase();
    }
    else if (!client.getuflag())
       sendMsgToClient(client.getClinetFd(), "Username ain't correct, try again ... (in a the form above)\n");
    if (client.getuflag())
    {
        const char msg[179] = AUTHED;
        sendMsgToClient(client.getClinetFd(), msg);
    }
    return ;
}

