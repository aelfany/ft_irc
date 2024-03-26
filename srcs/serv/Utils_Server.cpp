/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/03/26 09:45:57 by idryab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

void Servrr::trimSpaces(const std::string& str) {
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
        const char msg[49] = "Authentication success, Welcome again, BIG DOG!\n";
        sendMsgToClient(client.getClinetFd(), msg);
    }
    return ;
}

// char Servrr::auth(char *str) {

//     try {
//     trimSpaces(str);
//     if(args[0] == "PASS"  && args[1] == _password && pass_flag == false) {
//         pass_flag = true;
//         args[0].erase();
//         args[1].erase();
//     }
//     if (pass_flag == false) {
//         throw "YOU NEED PASSWORD";
//     }
//     if(args[0] == "NICK" && !args[1].empty() && pass_flag == true && !nick_flag) {
//         if(!args[1].empty())
//             nick_flag = true;   
//         args[0].erase();
//         args[1].erase();
//     }
//     std::cout << nick_flag << std::endl;
//     if (nick_flag == false) {
//         throw "YOU NEED NICK";
//     }
//     if(args[0] == "USER" && !args[1].empty() && nick_flag == true && pass_flag == true && !user_flag) {   
//         user_flag = true;
//         args[0].erase();
//         args[1].erase();
//     }
//     if (user_flag == false) {
//         throw "YOU NEED USER";
//     }
//     }
//     catch(const char *str)
//     {
//         std::cout << str << std::endl;
//     }
//     return 0;
// }
