/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/03/25 02:33:14 by idryab           ###   ########.fr       */
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
    std::cout << str << std::endl;
}

void  Servrr::auth(char *str, int client_sock_fd)
{
    trimSpaces(str);
    if(args[0] == "PASS"  && args[1] == _password && pass_flag == false)
    {
        pass_flag = true;
        args[0].erase();
        args[1].erase();
    }
    else if(args[0] == "NICK" && !args[1].empty() && pass_flag == true && !nick_flag)
    {
        nick_flag = true;   
        args[0].erase();
        args[1].erase();
    }
    else if(args[0] == "USER" && !args[1].empty() && nick_flag == true && pass_flag == true && !user_flag)
    {
        user_flag = true;
        args[0].erase();
        args[1].erase();
    }
    if (pass_flag == false)
        std::cout << "YOU NEED PASSWORD\n";
    else if (nick_flag == false)
        std::cout << "YOU NEED NICK\n";
    else if (user_flag == false)
        std::cout << "YOU NEED USER\n";
    if (user_flag == true)
    {
        const char msg[25] = "Welcome again, BIG DOG!\n";
        ssize_t bytes = send(client_sock_fd, msg, sizeof(msg), 0);
        if (bytes == -1)
            perror("send: ");
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
