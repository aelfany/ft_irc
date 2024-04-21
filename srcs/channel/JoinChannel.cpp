#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"
#include <iomanip>

#define RPL_JOINN(topic, nickname, channel) ":irc.idryab.chat 311 " + nickname + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPICC(mode, nickname, username, channel) ":" + nickname + "!" + username + "@127.0.0.1" + " JOIN " + channel + " " + mode +  "\r\n"
#define RPL_NAMREPLYY(listofnames, nickname, channel) ":irc.idryab.chat 353 " + nickname + " @ " + "" + channel + " :" + listofnames +"\r\n"
#define RPL_ENDOFNAMESS(nickname, channel) ":irc.idryab.chat 366 " + nickname + " " + channel + " :End of /NAMES list.\r\n"


bool alreadyAmember(int clientfd, Channel channel)
{
    map_users mapOfClients = channel.getUsersMap();
    map_users::iterator iter;
    for(iter = mapOfClients.begin(); iter != mapOfClients.end(); iter++)
    {
        if (iter->second.getClinetFd() == clientfd)
            return true;
    }
    return false;
}


std::string getListOfNames(map_users _users)
{
    std::string listOfNames;
    map_users::iterator iter;
    for(iter = _users.begin(); iter != _users.end(); iter++)
    {
        if (iter->first == true)
            listOfNames += "@" + iter->second.getNickName() + " ";
        else
            listOfNames += iter->second.getNickName() + " ";
    }
    return listOfNames;
}

void	Servrr::proccessChannels(int clientfd)
{
	std::stringstream chan(_result[1]);
	std::stringstream pass(_result[2]);
    std::string channel;
    std::string password;
    std::string nickname = getClientitoByfd(clientfd).getNickName();
    std::string channelTopic = "drug dealers";
    std::string listofnames;
    while (std::getline(chan, channel, ','))
	{
        if(channel[0] != '#' && channel != "JOIN")
            channel = "#"+channel;
        std::map<std::string, Channel>::iterator it = _channels.find(channel);
        if (it != _channels.end())
        {
            if (alreadyAmember(clientfd, it->second) == true)
            {
                std::cout << "Client " << nickname << " is already a member in this channel" << std::endl;
                return ;
            }
            if (it->second.getInvOnly() == true)
                return ;
            it->second.pushtomap(false, getClientitoByfd(clientfd));
            it->second.setusersSize(1);
            listofnames = getListOfNames(it->second.getUsersMap());
            sendMsgToClient(clientfd, RPL_JOINN(channelTopic, nickname, channel));
		    sendMsgToClient(clientfd, RPL_TOPICC("+", nickname, "DefaultUser", channel));
		    sendMsgToClient(clientfd, RPL_NAMREPLYY(listofnames, nickname, channel));
            sendMsgToClient(clientfd, RPL_ENDOFNAMESS(nickname, channel));
            return ;
        }
        listofnames = "@"+nickname;
		Channel newchannel(channel);
        newchannel.pushtomap(true, getClientitoByfd(clientfd));
		if (std::getline(pass, password, ','))
        {
		    newchannel.setPassword(password);
            newchannel.setPass(true);
        }
		_channels.insert(std::make_pair(channel, newchannel));
		sendMsgToClient(clientfd, RPL_JOINN(channelTopic, nickname, channel));
		sendMsgToClient(clientfd, RPL_TOPICC("+", nickname, "DefaultUser", channel));
		sendMsgToClient(clientfd, RPL_NAMREPLYY(listofnames, nickname, channel));
        sendMsgToClient(clientfd, RPL_ENDOFNAMESS(nickname, channel));
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