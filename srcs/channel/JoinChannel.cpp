#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"
#include <iomanip>

bool Servrr::alreadyAmember(int clientfd, Channel channel)
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

std::string     Channel::gettopicseter() {
    return topicseter;
}
std::string     Channel::gettopictime() {
    return topictime;
}

std::string     Channel::getTopic() {
    return _topic;
}

std::string getListOfNames(map_users _users)
{
    std::string listOfNames;
    map_users::iterator iter;
    for(iter = _users.begin(); iter != _users.end(); iter++)
    {
        if (iter->first.first == true)
            listOfNames += "@" + iter->second.getNickName() + " ";
        else
            listOfNames += iter->second.getNickName() + " ";
    }
    return listOfNames;
}

std::string Servrr::tolowercases(std::string _str1)
{
    size_t i = 0;
    std::string _lower = _str1;
    while (i < _str1.length())
    {
        _lower[i] = std::tolower(_str1[i]);
        i++;
    }
    return _lower;
}

std::string touppercases(std::string _str1)
{
    size_t i = 0;
    std::string _lower = _str1;
    while (i < _str1.length())
    {
        _lower[i] = std::toupper(_str1[i]);
        i++;
    }
    return _lower;
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
     std::string serverHostname = "127.0.0.";
    while (std::getline(chan, channel, ','))
	{
        if(channel[0] != '#' && channel != "join")
            channel = "#"+channel;
        std::string channellower = tolowercases(channel);
        std::map<std::string, Channel>::iterator it = _channels.find(channellower);
        if(it == _channels.end())
        {
            listofnames = "@"+nickname;
		    Channel newchannel(channellower);
            newchannel.pushtomap(true, getClientitoByfd(clientfd));
		    if (std::getline(pass, password, ','))
            {
		        newchannel.setPassword(password);
                newchannel.setPass(true);
            }
            newchannel.setChannelNameDisplay(channel);
            newchannel.setusersSize(1);
		    _channels.insert(std::make_pair(channellower, newchannel));
		    sendMsgToClient(clientfd, RPL_JOIN(nickname, nickname,channel, serverHostname));
		    // sendMsgToClient(clientfd, RPL_TOPICC("+o", nickname, nickname, channel));
            sendMsgToClient(clientfd, RPL_NAMREPLY(serverHostname, listofnames, channel, nickname));
            sendMsgToClient(clientfd, RPL_ENDOFNAMES(serverHostname, nickname, channel));
        }
        else if (it != _channels.end())
        {
            if (alreadyAmember(clientfd, it->second) == true)
            {
                std::cout << "Client " << nickname << " is already a member in this channel" << std::endl;
                return ;
            }
            if (it->second.getInvOnly() == true)
            {
                //<client> <channel> :Cannot join channel (+i)
                sendMsgToClient(clientfd, "473 " + nickname + " " +  it->second.getChannelNameDisplay() + " :Cannot join channel (+i)\r\n");
                return ;
            }
            it->second.pushtomap(false, getClientitoByfd(clientfd));
            it->second.setusersSize(1);
            listofnames = getListOfNames(it->second.getUsersMap());

            sendMsgToClient(clientfd, RPL_JOIN(nickname, nickname, it->second.getChannelNameDisplay(), serverHostname));
            broadcastMessage(it->second, RPL_JOIN(nickname, nickname,it->second.getChannelNameDisplay(), serverHostname), clientfd);
            
            sendMsgToClient(clientfd, RPL_NAMREPLY(serverHostname, listofnames, it->second.getChannelNameDisplay(), nickname));
            broadcastMessage(it->second, RPL_NAMREPLY(serverHostname, listofnames, it->second.getChannelNameDisplay(), nickname), clientfd);

            sendMsgToClient(clientfd, RPL_ENDOFNAMES(serverHostname, nickname, it->second.getChannelNameDisplay()));
            broadcastMessage(it->second, RPL_ENDOFNAMES(serverHostname, nickname, it->second.getChannelNameDisplay()), clientfd);
        }
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
    if (tolowercases(_result[0]) == "join")
    {
        if (_result[1].empty())
            sendMsgToClient(clientfd, ERR_NEEDMOREPARAMS(getClientitoByfd(clientfd).getNickName(), "join"));
        else
        {
			proccessChannels(clientfd);
            ShowChannels(_channels);//display channels' users and thier info.
        }
        _result.clear();
    }
}