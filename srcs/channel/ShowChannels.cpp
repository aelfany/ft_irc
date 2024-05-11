#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"
#include <iomanip>


void printChannelClients(Channel &obj)
{
    map_users usersMap = obj.getUsersMap();
    map_users::iterator it = usersMap.begin();
    std::cout << "+-------------------------------------------------+\n";
    for (; it != usersMap.end(); ++it)
    {
        std::cout << "	Nickname: " << it->second.getNickName() << std::endl;
        std::cout << " 	Sock_fd: " << it->second.getClinetFd() << std::endl;
        std::cout << "	Authentication: " << it->second.isAuthed() << std::endl;
        std::cout << "	Operator: " << it->first.first << std::endl;
        std::cout << "      ---------------------\n";
    }
}

void printRowChannels(const std::string& channel, const std::string& password)
{
    const int width = 50;
    std::cout << "+-------------------------------------------------+\n";
    std::cout << "| " << std::left << std::setw(width - 2) << "CHANNEL : " + channel << "|\n";
    std::cout << "| " << std::left << std::setw(width - 2) << "PASSWORD: " + password << "|\n";
}


void ShowChannels(std::map<std::string, Channel> ChannelsMap)
{
    std::map<std::string, Channel>::iterator it = ChannelsMap.begin();
    for (; it != ChannelsMap.end(); ++it)
    {
        printRowChannels(it->first, it->second.getPassword());
        printChannelClients(it->second);
    }
	std::cout << "+-------------------------------------------------+\n";
}