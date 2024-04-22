#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"
#include <iomanip>

std::string Channel::getAllUsers() {
    std::string users;
    map_users::iterator it = _users.begin();
    std::cout << it->second.getNickName() << std::endl; 
        puts("$$$$$$$$$$$$$$$");
    for(; it != _users.begin(); it++) {
        puts("$$$$$$$$$$$$$$$");
        if(getPrvBynickname(it->second.getNickName()) == true)
            users += "@" + it->second.getNickName();
        else
            users += it->second.getNickName();
        users += " ";
    }
    return users;
}

void	Servrr::proccessChannels(int clientfd)
{
    std::string ip = inet_ntoa(_addr.sin_addr);
	std::stringstream chan(_result[1]);
	std::stringstream pass(_result[2]);
    std::string channel;
    std::string password;
    std::string nickname = getClientitoByfd(clientfd).getNickName();
    std::string serverHostname = "127.0.0.1";
    std::string channelTopic = "drug dealers";
    while (std::getline(chan, channel, ','))
	{
        if(channel[0] == '#' && channel != "JOIN")
            channel = channel.substr(1, std::string::npos);
        
        //this can be removed, cuz map container handle it(if we push the same key it will keep the old one).
        std::map<std::string, Channel>::iterator it = _channels.find(channel);
		Channel newchannel(channel);
        if (it != _channels.end())
        {
            // map_users mapOfClients = it->second.getUsersMap();
            // map_users::iterator itr = mapOfClients.find(nickname);
            // if (itr != mapOfClients.end())
            // {
            //     std::cout << "Client is already a member in this channel" << std::endl;
            //     return ;
            // }
            if (it->second.getInvOnly() == true)
                return ;
            it->second.pushtomap(false, getClientitoByfd(clientfd));
            it->second.setusersSize(1);
            return ;
        }
        // getClientitoByfd(clientfd).setOperator(true);
        newchannel.pushtomap(true, getClientitoByfd(clientfd));
		if (std::getline(pass, password, ','))
        {
		    newchannel.setPassword(password);
            newchannel.setPass(true);
        }
            channel = "#" + channel;
            _channels.insert(std::make_pair(channel, newchannel));
            sendMsgToClient(clientfd, RPL_JOIN(nickname, "temp",channel, ip));
            sendMsgToClient(clientfd, RPL_NAMREPLY(serverHostname, "users",channel, nickname));
            sendMsgToClient(clientfd, RPL_ENDOFNAMES(serverHostname, nickname, channel));

	}
}

void    Servrr::parseJoinCommand(const std::string& command)
{
  std::stringstream iss(command);
	std::string part;

	while (iss >> part)
        _result.push_back(part);
    _result.push_back("");
}

void    Servrr::createChannel(std::string command, int clientfd)
{
    parseJoinCommand(command);
    if (_result[0] == "JOIN")
    {
        if (_result[1].empty())
            sendMsgToClient(clientfd, "Plz, Provide a channnel name\n");
        else
        {
			proccessChannels(clientfd);
            ShowChannels(_channels);//display channels' users and thier info.
        }
        _result.clear();
    }
}