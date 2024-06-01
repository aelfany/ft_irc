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

void	Servrr::proccessChannels(int clientfd, std::string nick)
{
	std::stringstream chan(_result[1]);
	std::stringstream pass(_result[2]);
    std::string channel;
    std::string password;
    std::string nickname = getClientitoByfd(clientfd).getNickName();
    std::string channelTopic = "drug dealers";
    std::string listofnames;
     std::string serverHostname = getClientitoByfd(clientfd).getipaddr();
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
            if(it->second.getUserLimit() == true)
            {
                if(it->second.getusersSize() >= it->second.getlimit())
                {
                    sendMsgToClient(clientfd, "471 " + nickname + " " +  it->second.getChannelNameDisplay() + " :Cannot join channel, it's full (+l)\r\n");
                    return ;
                }
            }
            if (it->second.getInvOnly() == true && it->second.isInvited(clientfd) == false)
            {
                //<client> <channel> :Cannot join channel (+i)
                sendMsgToClient(clientfd, "473 " + nickname + " " +  it->second.getChannelNameDisplay() + " :Cannot join channel (+i)\r\n");
                return ;
            }
            if(it->second.getPass() == true)
            {
                std::getline(pass, password, ',');
                if (it->second.getPassword() != password)
                {
                    sendMsgToClient(clientfd, ERR_BADCHANNELKEY(nickname, channel));
                    return ;
                }
            }
            it->second.pushtomap(false, getClientitoByfd(clientfd));
            it->second.setusersSize(1);
            it->second.removeInvitedClient(clientfd);
            listofnames = getListOfNames(it->second.getUsersMap());

            sendMsgToClient(clientfd, RPL_JOIN(nickname, nickname, it->second.getChannelNameDisplay(), serverHostname));
            broadcastMessage(it->second, RPL_JOIN(nickname, nickname,it->second.getChannelNameDisplay(), serverHostname), clientfd);
            
            sendMsgToClient(clientfd, RPL_NAMREPLY(serverHostname, listofnames, it->second.getChannelNameDisplay(), nickname));
            // broadcastMessage(it->second, RPL_NAMREPLY(serverHostname, listofnames, it->second.getChannelNameDisplay(), nickname), clientfd);

            sendMsgToClient(clientfd, RPL_ENDOFNAMES(serverHostname, nickname, it->second.getChannelNameDisplay()));
            
            broadcastMessage(it->second, RPL_ENDOFNAMES(serverHostname, nickname, it->second.getChannelNameDisplay()), clientfd);
            sendMsgToClient(clientfd,RPL_TOPICDISPLAY(host, nick,it->second.getChannelNameDisplay(),it->second.getTopic()));
            sendMsgToClient(clientfd,RPL_TOPICWHOTIME(host, it->second.getChannelNameDisplay(), nick,it->second.gettopicseter(),getClientitoByIndex(i-1).getUserName(),it->second.gettopictime()));
        }
	}
    getClient(nick).pushChannel(channel);
}

void    Servrr::parseJoinCommand(const std::string& command)
{
  std::stringstream iss(command);
	std::string part;

	while (iss >> part)
        _result.push_back(part);
    _result.push_back("");
}

void    Servrr::createChannel(std::string command, int clientfd, std::string nick)
{
    parseJoinCommand(command);
    if (tolowercases(_result[0]) == "join")
    {
        if (_result[1].empty())
            sendMsgToClient(clientfd, ERR_NEEDMOREPARAMS(getClientitoByfd(clientfd).getNickName(), "join"));
        else
        {
			proccessChannels(clientfd, nick);
            ShowChannels(_channels);//display channels' users and thier info.
        }
        _result.clear();
    }
}