#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"
#include <iomanip>



// #define RPL_JOINN(nickname, hostname) ":" + nickname + " has joined (~" + nickname + "@" + hostname + ")\r\n"

#define RPL_JOINN(serverHostname, channel) (":" + serverHostname + " JOIN :#" + channel + "\r\n")
#define RPL_TOPICC(serverHostname, nickname, channel, channelTopic) ":" + serverHostname + " 332 " + nickname + " #" + channel + " :" + channelTopic + "\r\n"
#define RPL_NAMREPLYY(serverHostname, nickname, channel) ":" + serverHostname + " 353 " + nickname + " = #" + channel + " :" + nickname + " @otheruser\r\n"
#define RPL_ENDOFNAMESS(serverHostname, nickname, channel) ":" + serverHostname + " 366 " + nickname + " #" + channel + " :End of /NAMES list.\r\n"


void	Servrr::proccessChannels(int clientfd)
{
	std::stringstream chan(_result[1]);
	std::stringstream pass(_result[2]);
    std::string channel;
    std::string password;
    std::string nickname = getClientitoByfd(clientfd).getNickName();
    std::string serverHostname = "127.0.0.1";
    while (std::getline(chan, channel, ','))
	{
        if(channel[0] == '#' && channel != "JOIN")
            channel = channel.substr(1, std::string::npos);
        
        //this can be removed, cuz map container handle it(if we push the same key it will keep the old one).
        std::map<std::string, Channel>::iterator it = _channels.find(channel);
		Channel newchannel(channel);
        if (it != _channels.end())
        {
            map_users mapOfClients = it->second.getUsersMap();
            map_users::iterator itr = mapOfClients.find(nickname);
            if (itr != mapOfClients.end())
            {
                std::cout << "Client is already a member in this channel" << std::endl;
                return ;
            }
            if (it->second.getInvOnly() == true)
                return ;
            if (it->second.getusersSize() >= 3)
            {
                std::cout << it->first << " channel is full" << std::endl;
                return ;
            }
            sendMsgToClient(clientfd, RPL_JOINN(nickname, channel));
            it->second.pushtomap(nickname, getClientitoByfd(clientfd));
            it->second.setusersSize(1);
            return ;
        }
        newchannel.pushtomap(nickname, getClientitoByfd(clientfd));
		if (std::getline(pass, password, ','))
        {
		    newchannel.setPassword(password);
            newchannel.setPass(true);
        }
		_channels.insert(std::make_pair(channel, newchannel));
        std::string channelTopic = "drug dealers";
		sendMsgToClient(clientfd, RPL_JOINN(nickname, serverHostname));
		sendMsgToClient(clientfd, RPL_TOPICC(serverHostname, nickname, channel, channelTopic));
		sendMsgToClient(clientfd, RPL_NAMREPLYY(serverHostname, nickname, channel));
        sendMsgToClient(clientfd, RPL_ENDOFNAMESS(serverHostname, nickname, channel));
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