#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"
#include <iomanip>

void	Servrr::proccessChannels(std::string &ch, int clientfd)
{
	std::istringstream chan(ch);
	std::istringstream pass(*(&ch+1));
    std::string channel;
    std::string password;
    while (std::getline(chan, channel, ','))
	{
		Channel newchannel(channel);
		std::getline(pass, password, ',');
		newchannel.setPassword(password);
		_channels.insert(std::make_pair(channel, newchannel));
		std::string msg = "Awosome, you just create " + channel + " channel, Enjoy!\n";
		sendMsgToClient(clientfd, msg.c_str());
	}
}

std::vector<std::string> parseJoinCommand(const std::string& command)
{
    std::vector<std::string> result;
    std::istringstream iss(command);
	std::string part;
	
	while (iss >> part)
		result.push_back(part);
    return result;
}

void printRowChannels(const std::string& channel, const std::string& password)
{
    const int width = 50;
    std::cout << "+-------------------------------------------------+\n";
    std::cout << "| " << std::left << std::setw(width - 2) << "CHANNEL : " + channel << "|\n";
    std::cout << "| " << std::left << std::setw(width - 2) << "PASSWORD: " + password << "|\n";
}

void    Servrr::createChannel(char *command, int clientfd)
{
    std::vector<std::string> parsedcmd;

    parsedcmd = parseJoinCommand(command);
    if (parsedcmd[0] == "JOIN")
    {
        if (parsedcmd[1].empty())
            sendMsgToClient(clientfd, "Plz, Provide a channnel name\n");
        else
        {
			proccessChannels(parsedcmd[1], clientfd);
			std::map<std::string, Channel>::iterator it;
			for (it = _channels.begin(); it != _channels.end(); ++it)
				printRowChannels(it->first, it->second.getPassword());
			std::cout << "+-------------------------------------------------+\n";
        }
    }
}