/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idryab <idryab@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/13 02:48:35 by idryab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"


//Sun Apr 21 17:43:10 2024
Channel & Servrr::getChannel(std::string channel) {
    std::map<std::string, Channel>::iterator it = _channels.find(tolowercases(channel));
    if (it == _channels.end())
        throw "No channel found";
    return it->second;
}
void Servrr::command(std::string buffer, size_t i) {

    std::cout << buffer << "\n";
    std::string s = inet_ntoa(_addr.sin_addr);
    std::string channel;
    std::string nick = getClientitoByIndex(i-1).getNickName();
    Channel a;
    trimSpaces(buffer,false);
    args[0] = tolowercases(args[0]);
    if(getClientitoByIndex(i-1).isAuthed() == false) {
        auth2(buffer, getClientitoByIndex(i-1));
    }
    else {
        if(args[0] == "pass") {
            if(args.size() < 2)
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,nick));
            else
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_ALREADYREGISTERED(s,nick));
        }
        else if(args[0] == "join") {
            trimSpaces(buffer,true);
            if(args.size() < 2)
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,nick));
            else
                createChannel(buffer, getClientitoByIndex(i-1).getClinetFd());
        }
        else if(args[0] == "topic") {
            Topic(nick,i);
        }
        else if(args[0] == "mode")
        {
            if (args.size() < 3)
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(nick, "MODE"));
                args.clear();
                return ;
            }
            try {
                Channel &mode = getChannel(args[1]);
                channel = getClientitoByIndex(i-1).getNickName();
                std::cout << "channel found secssusfly '*`" << std::endl;
                if(SET_I) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else
                        mode.setInvOnly(true);
                }
                else if(REMOVE_I) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else    
                        mode.setInvOnly(false);
                }
                if(SET_T) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else    
                        mode.setTopc(true);       
                }
                else if(REMOVE_T) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else
                        mode.setTopc(false);
                }
                //**************************//
                //-------need parsing-------//
                if(SET_K) {
                    if(mode.getPass() == true)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ALREADYSET(s));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else {
                        mode.setPassword(args[3]);
                        mode.setPass(true);
                    }
                }
                else if(REMOVE_K) {
                    if(mode.getPass() == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOPASSSET(s));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else {
                            mode.getPassword().erase();
                            mode.setPass(false);
                    }
                }
                //---------------------//
                if(SET_O) {
                    
                    puts("////////////////////////////////////////////////");   
                    if(args.size() < 4)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,channel));
                    else if(mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else {
                        try {
                            mode.getUserBynickname(args[3]);
                        }
                        catch(const char *) {
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(s,channel));
                        }
                    }
                    printf("////////////////////////////////////////////////\n");   
                    std::cout << "@->>" << mode.getPrvBynickname(channel) << std::endl; 
                    std::cout << "@->>" << mode.getPrvBynickname(channel) << std::endl; 
                    mode.setPrvByNickname(channel, false, mode.getUserBynickname(channel));
                    printf("////////////////////////////////////////////////\n");   
                    /***/
                    
                }
                else if(REMOVE_O) {
                    /***/
                    if(args.size() < 4)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(s,channel));
                    else if(mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(s,channel));
                    else {
                        try {
                            mode.getUserBynickname(args[3]);
                        }
                        catch(const char *) {
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(s,channel));
                        }
                    }
                    std::cout << "@->>" << mode.getPrvBynickname(channel) << std::endl; 
                    mode.setPrvByNickname(channel, false, mode.getUserBynickname(channel));
                    std::cout << "@->>" << mode.getPrvBynickname(channel) << std::endl; 
                    puts("----------------------------------------");
                }
                if(SET_L) {
                    std::stringstream ss(args[3]);
                    size_t limit;
                    ss >> limit;
                    mode.setLimit(limit);
                    mode.setUserLimit(true);
                }
                else if(REMOVE_L) {
                    mode.setUserLimit(false);
                    mode.setLimit(-1);
                }
            }
            catch(...) {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(s, nick, "#" + args[1]));
            }
        }
        else if (args[0] == "privmsg")
        {
            std::string _message;
            try
            {
                if (!alreadyAmember(getClientitoByIndex(i-1).getClinetFd(), getChannel(args[1])))
                {
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(nick, args[0]));
                    args.clear();
                    return ;
                }
            }
            catch(...)
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(nick, args[0]));
                std::cerr << "You're not on that channel\n";
            }
            for(size_t i = 0; i < args.size(); i++)
            {
                if(i > 1)
                {
                    if (args[i][0] == ':')
                        _message = args[i].substr(1) + " ";
                    else
                        _message += args[i] + " ";
                }
            }
            sendmessage(getClientitoByIndex(i-1), args[1], _message);
        }
        else if (args[0] == "invite")
        {
            if (!alreadyAmember(getClientitoByIndex(i-1).getClinetFd(), getChannel(args[2])))
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(nick, args[0]));
                return ;
            }
            if(args.size() < 3)
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(nick, args[2]));
                return ;
            }
            while(i < _clients.size())
            {
                if (_clients[i].getNickName() == args[1])
                {
                    //otman!~otman@172.0.0.1 INVITE sabir :#osabir
                    std::string msg = args[1]+"!~"+nick+"@127.0.0.1 INVITE "+args[1]+" :"+args[2]+"\r\n";
                    sendMsgToClient(_clients[i].getClinetFd(), msg);
                    // sendMsgToClient(_clients[i].getClinetFd(), RPL_INVITING());
                    break ;
                }
                i++;
            }
        }
        else if (args[0] == "part")
        {
            try
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ":" + nick + "!~" + nick + "@127.0.0.1 QUIT :Remote host closed the connection\r\n");
                removeFromChannel(getClientitoByIndex(i-1).getClinetFd());
                Channel &obj = getChannel(args[1]);
                if (obj.getusersSize() == 0)
                    eraseChannel(tolowercases(args[1]));
            }
            catch(...)
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(nick, args[1]));
                return ;
            }
            
        }
        else if (args[0] == "quit")
        {
            int client_sock_fd =  getClientitoByIndex(i-1).getClinetFd();
                // If client disconnected print this==> && close its socket && erase its data in our vector
                std::cout << "Client "<< client_sock_fd << " disconnected" << std::endl;
                close(client_sock_fd);
                _fds.erase(_fds.begin() + _index);
                removeClient(_index-1);
                //remove client from channel && send message to channels he joined that client has been deconnected.
                removeFromChannel(client_sock_fd);
                _index--;
                channelsMap::iterator it = _channels.begin();
                while (it != _channels.end())
                {
                    if (it->second.getusersSize() == 0)
                        it = _channels.erase(it);
                    else
                        ++it;
                }
    }
    args.clear();
}
}
