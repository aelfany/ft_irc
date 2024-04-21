/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/04/21 10:43:04 by idryab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"
#include "../../include/replies.hpp"

void Servrr::trimSpaces(const std::string& str, bool x)
{
    std::stringstream splt(str);
    std::string s;
    if(x)
        args.clear();
    while(splt >> s) {
        if(!(x && s == "#")) {
            // std::cout << s << std::endl;
            args.push_back(s);
        }
    }
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
    trimSpaces(str,false);
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
    if(client.isAuthed())
    {
        std::string nickname = "сукаблять";
        sendMsgToClient(client.getClinetFd(), RPL_AUTHENTICATED(nickname));
    }
    return ;
}


void    broadcastMessage(Channel _channel, std::string _message, int _clientfd)
{
    map_users mapOfClients = _channel.getUsersMap();
    map_users::iterator iter;
    for(iter = mapOfClients.begin(); iter != mapOfClients.end(); iter++)
    {
        if (iter->second.getClinetFd() != _clientfd)
            sendMsgToClient(iter->second.getClinetFd(), _message);
    }
}

void    Servrr::sendmessage(std::string _destination, std::string _message, int clientfd)
{
    std::map<std::string, Channel>::iterator it = _channels.find(_destination);
    if (it != _channels.end())
    {
        //Broadcast the message to all clients in this channel
        broadcastMessage(it->second, _message, clientfd);
        std::cout << "destination is a channel" << std::endl;
    }
    else
    {
        std::cout << "destination is solo client" << std::endl;
        for (size_t i = 0; _clients.size(); i++)
        {
            if (_clients[i].getNickName() == _destination)
            {
                std::cout << "You're about to send msg to " << _clients[i].getNickName() << "..." <<std::endl;
            }
        }
        // sendMsgToClient(iter->second.getClinetFd(), _message);
    }
    std::cout << _message << std::endl;
}