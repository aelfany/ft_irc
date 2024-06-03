/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/06/03 15:35:46 by abelfany         ###   ########.fr       */
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

int Servrr::parsNick(clientito& client) {
    std::string str = "@&#:1234567890";
    for(size_t a = 0; a < _clients.size(); a++) {
        if(args[1] == _clients[a].getNickName()) {
            client.setnflag(false);
            sendMsgToClient(client.getClinetFd(), ERR_NICKNAMEINUSE(args[1]));
            return 0;
        }
    }
    if(str.find(args[1][0]) != std::string::npos)
        client.setnflag(false);
    return 1;
}

void	Servrr::auth2(std::string str, clientito& client)
{
    (void)str;
    std::string nick = client.getNickName();
    // std::cout << "\033[0;32m" << << "\033[0m" << std::endl;
    if (args.size() < 2) {
        if(args[0] == "nick")
            sendMsgToClient(client.getClinetFd(), RPL_NONICKNAMEGIVEN(nick, host));
        else
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
            int x = parsNick(client);
            if(client.getnflag())
                client.setNickName(args[1]);
            else if(x)
                sendMsgToClient(client.getClinetFd(), RPL_ERRONEUSNICKNAME(host, nick));
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
        time_t now = time(0);
        std::string tmp = ctime(&now);
        std::string nick = client.getNickName();
        std::string host = client.getipaddr();
        std::string time = tmp;
        sendMsgToClient(client.getClinetFd(), RPL_WELCOME(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_YOURHOST(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_CREATED(nick, host, time));
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
    std::string senderUsername = client.getUserName();
    if(_channels.size() == 0 && reciever[0] == '#')
    {
        sendMsgToClient(client.getClinetFd(), ERR_NOSUCHCHANNEL(senderNick, senderNick, reciever));
        return ;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(tolowercases(reciever));
    if (it != _channels.end())
    {
        if(!alreadyAmember(client.getClinetFd(), it->second))
        {
            sendMsgToClient(client.getClinetFd(), ERR_CANNOTSENDTOCHAN(senderNick, it->second.getChannelNameDisplay()));
            return ;
        }
        broadcastMessage(it->second, ":" + senderNick + "!~" + senderUsername + "@127.0.0.1 PRIVMSG " + reciever + " :" + _message + "\r\n", client.getClinetFd());
    }
    else if(reciever[0] == '#')
    {
        sendMsgToClient(client.getClinetFd(), ERR_NOSUCHCHANNEL(senderNick, senderNick, it->second.getChannelName()));
        return ;
    }
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