/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 22:23:41 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/24 19:14:47 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/client.hpp"
#include "../../include/Server.hpp"

void Channel::updateNickname(std::string _nickname, std::string oldnick, bool prv, clientito & obj) {
    
    (void)obj;
    (void)_nickname;
    std::map<std::pair<bool, int>, clientito>::iterator it = _users.begin();
    for (; it != _users.end(); ++it) {
        if (it->second.getNickName() == oldnick) {
            clientito tempObj = it->second;
            int x = it->second.getClinetFd();
            std::pair<bool, int> newKey(prv, x);
            _users[it->first] = obj;
            break; 
        }
    }
}
