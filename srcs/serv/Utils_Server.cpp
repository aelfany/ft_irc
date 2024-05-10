/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 02:17:42 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/07 17:13:22 by idryab           ###   ########.fr       */
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
    std::cout << "*******\n" << args.size() << "\n*******" << std::endl;
    if (args.size() < 2)
    {
        args.clear();
        return ;
    }
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
    if(args[0] == "NICK" && !args[1].empty() && client.getpflag() && !client.getnflag())
    {
        std::cout << "OK" << std::endl;
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
        std::cout << "OK" << std::endl;
        client.setuflag(true);
        args.clear();
    }
    else if (!client.getuflag())
       sendMsgToClient(client.getClinetFd(), "Username ain't correct, try again ... (in a the form above)\n");
    if(client.isAuthed())
    {   
        std::string nick = client.getNickName();
        std::string host = "127.0.0.1";
        sendMsgToClient(client.getClinetFd(), RPL_WELCOME(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_YOURHOST(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_CREATED(nick, host));
        sendMsgToClient(client.getClinetFd(), RPL_MYINFO(nick, host));
        // std::string nickname = "сукаблять";
        // sendMsgToClient(client.getClinetFd(), RPL_AUTHENTICATED(nickname));
    }
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