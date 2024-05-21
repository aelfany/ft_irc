/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/20 23:35:43 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/client.hpp"


Channel & Servrr::getChannel(std::string channel) {
    std::map<std::string, Channel>::iterator it = _channels.find(tolowercases(channel));
    if (it == _channels.end())
        throw "No channel found";
    return it->second;
}
void Servrr::command(std::string buffer, size_t i) {
    char buf[256];
    if(gethostname(buf,sizeof(buf)))
        return ;
    host = buf;
    std::cout << host << std::endl;
    std::string channel;
    Channel a;
    trimSpaces(buffer,false);
    args[0] = tolowercases(args[0]);
    if(getClientitoByIndex(i-1).isAuthed() == false) {
        // std::cout << "\033[0;31m" << "->>> ##################" << "\033[0m" << std::endl;
        auth2(buffer, getClientitoByIndex(i-1));
    }
    else {
        std::string nick = getClientitoByIndex(i-1).getNickName();
        if(args[0] == "pass") {
            if(args.size() < 2)
            {
                // std::cout << "\033[0;31m" << "1 = ##################" << "\033[0m" << std::endl;
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,nick));
            }
            else
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_ALREADYREGISTERED(host,nick));
        }
        else if(args[0] == "join") {
            trimSpaces(buffer,true);
            if(args.size() < 2) {
                // std::cout << "\033[0;31m" << "2 = ##################" << "\033[0m" << std::endl;
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,nick));
            }
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
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else
                    {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+i"));
                        mode.setInvOnly(true);
                    }
                }
                else if(REMOVE_I) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else    
                    {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-i"));
                        mode.setInvOnly(false);
                    }
                }
                if(SET_T) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else    
                    {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+t"));
                        mode.setTopc(true);       
                    }
                }
                else if(REMOVE_T) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else
                    {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-t"));
                        mode.setTopc(false);
                    }
                }
                if(SET_K) {
                    if(mode.getPass() == true)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ALREADYSET(nick));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+k " + args[3]));
                        mode.setPassword(args[3]);
                        mode.setPass(true);
                    }
                }
                else if(REMOVE_K) {
                    if(mode.getPass() == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOPASSSET(nick));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-k " + args[3]));
                        mode.getPassword().erase();
                        mode.setPass(false);
                    }
                }
                //---------------------//
                if(SET_O) {
                    
                    if(args.size() < 4)
                    {
                        // std::cout << "\033[0;31m" << "4 = ##################" << "\033[0m" << std::endl;
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,channel));
                    }
                    else if(mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else {
                        try {
                            mode.getUserBynickname(args[3]);
                        }
                        catch(const char *) {
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(host,channel));
                        }
                        mode.setPrvByNickname(args[3], true, mode.getUserBynickname(channel));
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+o " + args[3]));
                    }    
                }
                else if(REMOVE_O) {
                    /***/
                    if(args.size() < 4)
                    {
                        // std::cout << "\033[0;31m" << "5 = ##################" << "\033[0m" << std::endl;
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,channel));
                    }
                    else if(mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,channel));
                    else {
                        try {
                            mode.getUserBynickname(args[3]);
                        }
                        catch(const char *) {
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(host,channel));
                        }
                        mode.setPrvByNickname(args[3], false, mode.getUserBynickname(channel));
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-o " + args[3]));
                    }
                }
                if(SET_L) {
                    std::stringstream ss(args[3]);
                    size_t limit;
                    ss >> limit;
                    mode.setLimit(limit);
                    mode.setUserLimit(true);
                    SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+l " + args[3]));
                }
                else if(REMOVE_L) {
                    mode.setUserLimit(false);
                    mode.setLimit(-1);
                    SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-l"));
                }
            }
            catch(...) {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, args[1]));
            }
        }
        else if (args[0] == "privmsg")
        {
            std::string _message;
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
            if(args.size() < 3)
            {
                // std::cout << "\033[0;31m" << "6 = ##################" << "\033[0m" << std::endl;
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(nick, args[2]));
                return ;
            }
            if (!alreadyAmember(getClientitoByIndex(i-1).getClinetFd(), getChannel(args[2])))
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(nick, args[0]));
                return ;
            }
            while(i < _clients.size())
            {
                if (_clients[i].getNickName() == args[1])
                {
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_INVITING(host, nick, args[1], args[2]));
                    sendMsgToClient(_clients[i].getClinetFd(), RPL_INVITE(nick, _clients[i].getUserName(), host, args[1], args[2]));
                    Channel &obj = getChannel(args[2]);
                    obj.setinvited(_clients[i].getClinetFd());
                    break ;
                }
                i++;
            }
        }
        else if (args[0] == "part")
        {
            try
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_QUIT(nick,host, "good bye"));
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_PART(nick,getClientitoByIndex(i-1).getUserName(), host, args[1]));
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
                std::cout << "Client "<< client_sock_fd << " disconnected" << std::endl;
                close(client_sock_fd);
                _fds.erase(_fds.begin() + _index);
                removeClient(_index-1);
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
