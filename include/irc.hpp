/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:22:01 by abelfany          #+#    #+#             */
/*   Updated: 2024/03/24 00:46:01 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.hpp"
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <netinet/in.h>

class irc {
    private :
        
        std::string pass;
        bool pass_flag;
        bool nick_flag;
        bool user_flag;
    public :
        irc() : pass_flag(false), nick_flag(false), user_flag(false) {} ;
        char auth(char *str);
        void trimSpaces(const std::string& str);
};


#endif