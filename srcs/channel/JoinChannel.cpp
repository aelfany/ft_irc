#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"
#include <iomanip>

void	Servrr::proccessChannels(int clientfd)
{
	  std::stringstream chan(_result[1]);
	  std::stringstream pass(_result[2]);
    std::string channel;
    std::string password;
    while (std::getline(chan, channel, ','))
	{
		Channel newchannel(channel);
		if (std::getline(pass, password, ','))
		    newchannel.setPassword(password);
		_channels.insert(std::make_pair(channel, newchannel));
		std::string msg = "Awosome, you just create " + channel + " channel, Enjoy!\n";
		sendMsgToClient(clientfd, msg.c_str());
	}
}

void    Servrr::parseJoinCommand(const std::string& command)
{
  std::stringstream iss(command);
	std::string part;

	while (iss >> part)
    {
        _result.push_back(part);
    }
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