/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 01:32:54 by abelfany          #+#    #+#             */
/*   Updated: 2024/06/01 15:08:00 by abelfany         ###   ########.fr       */
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

std::vector<std::string> clientito::getChannels() {
    return channels;
}

int Servrr::modeSplit(std::string arg, size_t i) {
    char x;
    std::string res;
    int index = 3;
    for(int a = 1; arg[a]; a++) {
        if(arg[a] != '-' && arg[a] != '+' && arg[a] != 'i' 
            && arg[a] != 't' && arg[a] != 'o' && arg[a] != 'k' && arg[a] != 'l' && arg[a] != 's' && arg[a] != 'n') {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_UNKNOWNMODE(host,getClientitoByIndex(i-1).getNickName(), arg[a]));
                return 1;
            }
        if(arg[a] == '-' || arg[a] == '+')
            x = arg[a++];
        if(x == '-') {
            res = "-";
            res += arg[a];
        }
        else {
            res = "+";
            res += arg[a];
        }
        if(arg[a] == 'o' || arg[a] == 'k' || arg[a] == 'l')
            mod.push_back(std::make_pair(res,args[index++]));
        else
            mod.push_back(std::make_pair(res,""));
    }
    return 0;
}

int Servrr::checkNick(clientito& client) {
    std::string str = "@&#:1234567890";
    std::string s = "_";
    for(size_t a = 0; a < _clients.size(); a++) {
        if(args[1] == _clients[a].getNickName()) {
            sendMsgToClient(client.getClinetFd(), ERR_NICKNAMEINUSE(args[1]));
            return 1;
        }
    }
    if(str.find(args[1][0]) != std::string::npos)
        return 0;
    if(s.find(args[1][strlen(args[1].c_str()) - 1]) != std::string::npos)
        return 3;
    return 2;
}


void Servrr::command(std::string buffer, size_t i) {
    char buf[256];
    int flag = 0;
    if(gethostname(buf,sizeof(buf)))
        return ;
    std::string nick = getClientitoByIndex(i-1).getNickName();
    clientito x = getClientitoByIndex(i-1);
    std::cout << host << std::endl;
    trimSpaces(buffer,false);
    std::string channel;
    std::string chanName;
    host = buf;
    Channel a;
    args[0] = tolowercases(args[0]);
    if(getClientitoByIndex(i-1).isAuthed() == false) {
        flag = 1;
        auth2(buffer, getClientitoByIndex(i-1));
        std::cout << getClientitoByIndex(i-1).isAuthed() << std::endl;
    }
    else {
        if(args[0] == "pass" || args[0] == "user") {
            flag = 1;
            if(args[0] == "pass" && args.size() < 2)
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,nick));
            else if(args[0] == "user" && args.size() < 5)
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,nick));
            else
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_ALREADYREGISTERED(host,nick));
        }
        if(args[0] == "nick") {
            flag = 1;
            int res = checkNick(getClientitoByIndex(i-1));
            if(res == 0 || res == 1 || res == 3) {
                if(res == 0) {
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ERRONEUSNICKNAME(host, nick));
                }
            }
            else {
                if(args.size() < 2 || args[1] == ":")
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NONICKNAMEGIVEN(nick, host));
                else {
                        std::string o = getClientitoByIndex(i-1).getNickName();
                        std::vector<std::string> vec = x.getChannels();
                        getClientitoByIndex(i-1).setNickName(args[1]);
                        std::vector<std::string>::iterator it = vec.begin();
                        for(; it != vec.end(); it++) {
                            Channel & chan = getChannel(*it);
                            chan.updateNickname(args[1],o,chan.getPrvBynickname(o), getClientitoByIndex(i-1));
                            SendToAll(chan, NICKNAME_RPLY(o,getClientitoByIndex(i-1).getUserName(),host,args[1]));
                        }
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NICKCHANGE(o,args[1],host));
                }
            }
        }
        else if(args[0] == "join") {
            flag = 1;
            trimSpaces(buffer,true);
            if(args.size() < 2)
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,nick));
            else
                createChannel(buffer, getClientitoByIndex(i-1).getClinetFd(), nick);
        }
        else if(args[0] == "topic") {
            flag = 1;
            Topic(nick,i);
        }
        else if(args[0] == "mode")
        {
            // for(size_t a = 0; a < mod.size(); a++) {
            //     std::cout << "mode ->> " << mod[a].first << "|" << mod[a].second << std::endl; 
            // }
            flag = 1;
            if (args.size() < 2) {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(nick, "MODE"));
                args.clear();
                return ;
            }
            try {
                Channel &mode = getChannel(args[1]);
                channel = getClientitoByIndex(i-1).getNickName();
                chanName = mode.getChannelNameDisplay();
                if (args.size() > 2) {
                    if(modeSplit(args[2], i) == 1) {
                        mod.clear();
                        args.clear();
                        return ;
                    }
                }
                std::cout << "channel found secssusfly '*`" << std::endl;
                for(size_t ind = 0; ind < mod.size(); ind++) {
                if(SET_I) {
                    if (mode.getInvOnly() == true)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ALLINV(args[1]));
                    else if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                    else
                    {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+i"));
                        mode.setInvOnly(true);
                    }
                }
                else if(REMOVE_I) {
                    if (mode.getInvOnly() == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOTINV(args[1]));
                    else if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                    else {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-i"));
                        mode.setInvOnly(false);
                    }
                }
                if(SET_T) {
                    
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                    else    
                    {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+t"));
                        mode.setTopc(true);       
                    }
                }
                else if(REMOVE_T) {
                    if (mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                    else
                    {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-t"));
                        mode.setTopc(false);
                    }
                }
                if(SET_K) {
                    if(args.size() < 4)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOKEY(nick, chanName, "k"));
                    else if(mode.getPassword() == mod[ind].second)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ALREADYSET(nick));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                    else {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+k " + mod[ind].second));
                        mode.setPassword(mod[ind].second);
                        mode.setPass(true);
                    }
                }
                else if(REMOVE_K) {
                    if(mode.getPass() == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOPASSSET(nick));
                    else if(mode.getPassword() != mod[ind].second)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_PASSNOTC(mod[ind].second));
                    else if (mode.getPrvBynickname(channel) == false) 
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                    else {
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-k " + mod[ind].second));
                        mode.getPassword().erase();
                        mode.setPass(false);
                    }
                }
                //---------------------//
                if(SET_O) {
                    try {
                        if(args.size() < 4)
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOKEY(nick, chanName, "o"));
                        else if(mode.getPrvBynickname(channel) == false)
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                        else if(mode.getPrvBynickname(mod[ind].second) == true)    
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_ALLOP(mod[ind].second));
                        else {
                            mode.getUserBynickname(mod[ind].second);
                            mode.setPrvByNickname(mod[ind].second, true);
                            SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+o " + mod[ind].second));
                        }
                    }    
                    catch(const char *) {
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(host,channel));
                    }
                }
                else if(REMOVE_O) {
                    if(args.size() < 4)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(host,"-o"));
                    else if(mode.getPrvBynickname(channel) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_CHANOPRIVSNEEDED(host,chanName));
                    else if(mode.getPrvBynickname(mod[ind].second) == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOTOP(mod[ind].second));
                    else {
                        try {
                            mode.getUserBynickname(mod[ind].second);
                            mode.setPrvByNickname(mod[ind].second, false);
                            SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-o " + mod[ind].second));
                        }
                        catch(const char *) {
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_WASNOSUCHNICK(host,channel));
                        }
                    }
                }
                if(SET_L) {
                    if(args.size() < 4) {
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOKEY(nick, chanName, "l"));
                        args.clear();
                        return ;
                    }
                    std::stringstream ss(mod[ind].second);
                    size_t limit;
                    ss >> limit;
                    if(mode.getlimit() == limit)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_LIMITSET(mod[ind].second));
                    else {
                        mode.setUserLimit(false);
                        mode.setLimit(limit);
                        mode.setUserLimit(true);
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "+l " + mod[ind].second));
                    }
                }
                else if(REMOVE_L) {
                    if(mode.getUserLimit() == false)
                        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_NOLIMITSET(nick));
                    else {
                        mode.setUserLimit(false);
                        mode.setLimit(-1);
                        SendToAll(mode, RPL_MODE(mode.getChannelName(), nick, "-l"));
                    }
                }
            }
            }
            catch(const char *) {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, args[1]));
                mod.clear();
                args.clear();
                return ;
            }
        }
        else if (args[0] == "kick") {
            flag = 1;
            kick(args[2], i);
        }
        else if (args[0] == "privmsg")
        {
            flag = 1;
            std::string _message;
            for(size_t i = 0; i < args.size(); i++)
            {
                if(i > 1)
                {
                    if (args[2][0] == ':')
                        _message += args[i] + " ";
                    else
                    {
                        _message = args[2];
                        break;
                    }
                }
            }
            if(_message[0] == ':')
                _message = _message.substr(1);
            sendmessage(getClientitoByIndex(i-1), args[1], _message);
        }
        else if (args[0] == "invite")
        {
            flag = 1;
            size_t indx = 0;
            try {
                if(args.size() < 3) {
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NEEDMOREPARAMS(nick, args[2]));
                    return ;
                }
                if (!alreadyAmember(getClientitoByIndex(i-1).getClinetFd(), getChannel(args[2]))) {
                    sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOTONCHANNEL(nick, args[0]));
                    return ;
                }
                while(indx < _clients.size())
                {
                    if (_clients[indx].getNickName() == args[1])
                    {
                            sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_INVITING(host, nick, args[1], args[2]));//who invite 
                            sendMsgToClient(_clients[indx].getClinetFd(), RPL_INVITE(nick, _clients[indx].getUserName(), host, args[1], args[2]));//c being invited
                            Channel &obj = getChannel(args[2]);
                            obj.setinvited(_clients[indx].getClinetFd());
                            break ;
                    }
                    indx++;
                }
            }
            catch(const char *)
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, args[1]));
                args.clear();
                return ;
            }
        }
        else if (args[0] == "part")
        {
            flag = 1;
            try
            {
                removeFromChannel(getClientitoByIndex(i-1).getClinetFd());
                Channel &obj = getChannel(args[1]);
                if (obj.getusersSize() == 0)
                    eraseChannel(tolowercases(args[1]));
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_QUIT(nick,host, "good bye"));
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), RPL_PART(nick,getClientitoByIndex(i-1).getUserName(), host, args[1]));
            }
            catch(const char *)
            {
                sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_NOSUCHCHANNEL(host, nick, args[1]));
                args.clear();
                return ;
            }
            
        }
        else if (args[0] == "quit")
        {
            flag = 1;
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
    if(args[0] == "pong")
        flag = 1;    
    if(flag == 0)
        sendMsgToClient(getClientitoByIndex(i-1).getClinetFd(), ERR_UNKNOWNCOMMAND(host, args[0]));
    args.clear();
    mod.clear();
}
}
