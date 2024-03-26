/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/03/25 10:18:37 by idryab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include <fcntl.h>
#include <poll.h>
#include <iostream>
#include <sstream>

void Servrr::trimSpaces(const std::string& str) {
    std::stringstream splt(str);
    std::string sv;
    std::string s;
    splt >> args[0];
    splt >> args[1];
}

void  Servrr::auth(char *str, int client_sock_fd)
{
    trimSpaces(str);
    if(args[0] == "PASS"  && args[1] == _password && pass_flag == false)
    {
        pass_flag = true;
        args[0].erase();
        args[1].erase();
        return ;
    }
    else if (pass_flag == false)
    {
        sendMsgToClient(client_sock_fd, "password ain't correct, try again ... (in a the form above)\n");
        return ;
    }
    if(args[0] == "NICK" && !args[1].empty() && pass_flag == true && !nick_flag)
    {
        nick_flag = true;   
        args[0].erase();
        args[1].erase();
        return ;
    }
    else if (nick_flag == false && pass_flag)
    {
        sendMsgToClient(client_sock_fd, "nickname ain't correct, try again ... (in a the form above)\n");
        return ;
    }
    if(args[0] == "USER" && !args[1].empty() && nick_flag == true && pass_flag == true && !user_flag)
    {
        user_flag = true;
        args[0].erase();
        args[1].erase();
    }
    else if (user_flag == false)
       sendMsgToClient(client_sock_fd, "username ain't correct, try again ... (in a the form above)\n");
    if (user_flag == true)
    {
        const char msg[25] = "Welcome again, BIG DOG!\n";
        sendMsgToClient(client_sock_fd, msg);
        pass_flag = false;
        nick_flag = false;
        user_flag = false;
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
