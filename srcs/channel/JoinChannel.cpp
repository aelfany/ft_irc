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
    std::string nickname = _clients[clientfd-4].getNickName();//this can cuz a problem, just temporary soluction.
        std::string serverHostname = "127.0.0.1";
    while (std::getline(chan, channel, ','))
	{
        if(channel[0] == '#' && channel != "JOIN")
            channel = channel.substr(1, std::string::npos);
        
        //this can be removed, cuz map container handle it(if we push the same key it will keep the old one).
        std::map<std::string, Channel>::iterator it = _channels.find(channel);
        if (it != _channels.end())
        {
            sendMsgToClient(clientfd, RPL_JOINN(nickname, channel));
            return ;
        }
		Channel newchannel(channel);
        newchannel.pushtomap(nickname, getClientito(clientfd-4));//this can cuz a problem, just temporary soluction.
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

	while (iss >> part) {
        std::cout << "************" << std::endl;
        std::cout << part << std::endl;
        _result.push_back(part);
    }
        std::cout << "************" << std::endl;
    _result.push_back("");
}

void printRowChannels(const std::string& channel, const std::string& password)
{
    const int width = 50;
    std::cout << "+-------------------------------------------------+\n";
    std::cout << "| " << std::left << std::setw(width - 2) << "CHANNEL : " + channel << "|\n";
    std::cout << "| " << std::left << std::setw(width - 2) << "PASSWORD: " + password << "|\n";
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
			std::map<std::string, Channel>::iterator it = _channels.begin();
			for (; it != _channels.end(); ++it)
				printRowChannels(it->first, it->second.getPassword());
			std::cout << "+-------------------------------------------------+\n";
        }
        _result.clear();
    }
}