/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/20 22:33:06 by abelfany         ###   ########.fr       */
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
            args.push_back(s);
        }
    }
}

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

void	Servrr::auth2(std::string str, clientito& client)
{
    (void)str;
    std::string nick = client.getNickName();
    if (args.size() < 2) {
        sendMsgToClient(client.getClinetFd(), ERR_NEEDMOREPARAMS(nick, "pass"));
        args.clear();
        return ;
    }
    if(args[0] == "pass"  && args[1] == _password && client.getpflag() == false) {
            client.setpflag(true);
            args.clear();
            return ;
    }
    else if (args[0] == "pass" && client.getpflag() == false) {
        sendMsgToClient(client.getClinetFd(), ERR_PASSWDMISMATCH(host, nick));
        args.clear();
        return ;
    }
    if(args[0] == "nick" && client.getpflag() && !client.getnflag()) {
            client.setnflag(true);
            parsNick(client);
            if(client.getnflag())
            client.setNickName(args[1]);
            args.clear();
            return ;
    }
    else if (client.getpflag() && !client.getnflag()) {
        args.clear();
        return ;
    }
    if(args[0] == "user" && !args[1].empty() && client.getpflag() && client.getnflag() && !client.getuflag()) {
        if(args.size() < 5)
            sendMsgToClient(client.getClinetFd(), ERR_NEEDMOREPARAMS(host, nick));
        else {
            client.setuflag(true);
            client.setUserName(args[1]);
            client.setRealName(args[4]);
            args.clear();
        }
    }
    else if (!client.getuflag()) {
        args.clear();
        return ;
    }
    if(client.isAuthed())
    {   
        std::string nick = client.getNickName();
        std::string host = client.getipaddr();
        sendMsgToClient(client.getClinetFd(), RPL_WELCOME(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_YOURHOST(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_CREATED(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_MYINFO(nick, host));
    }
    args.clear();
    return ;
}


void    Servrr::broadcastMessage(Channel _channel, std::string _message, int _clientfd)
{
    map_users mapOfClients = _channel.getUsersMap();
    map_users::iterator iter;
    for(iter = mapOfClients.begin(); iter != mapOfClients.end(); iter++)
    {
        if (iter->second.getClinetFd() != _clientfd)
            sendMsgToClient(iter->second.getClinetFd(), _message);
    }
}

void    Servrr::SendToAll(Channel _channel, std::string _message)
{
    map_users mapOfClients = _channel.getUsersMap();
    map_users::iterator iter;
    for(iter = mapOfClients.begin(); iter != mapOfClients.end(); iter++) {
        sendMsgToClient(iter->second.getClinetFd(), _message);
    }
}

void    Servrr::sendmessage(clientito &client, std::string reciever, std::string _message)
{
    std::string senderNick = client.getNickName();
    std::string senderUsername = client.getNickName();//don't forget to change this
    std::map<std::string, Channel>::iterator it = _channels.find(tolowercases(reciever));
    if (it != _channels.end())
        broadcastMessage(it->second, ":" + senderNick + "!~" + senderUsername + "@127.0.0.1 PRIVMSG " + reciever + " :" + _message + "\r\n", client.getClinetFd());
    else
    {
        size_t i = 0;
        while(i < _clients.size())
        {
            if (_clients[i].getNickName() == reciever)
            {
                sendMsgToClient(_clients[i].getClinetFd(), ":" + senderNick + "!~" + senderUsername + "@127.0.0.1 PRIVMSG " + reciever + " :" + _message + "\r\n");
                return ;
            }
            i++;
        }
    }
}