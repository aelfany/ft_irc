#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"


void    Servrr::createChannel(char *command, int clientfd)
{
    std::string fullcmd[2];
    std::stringstream ss(command);
    ss >> fullcmd[0];
    ss >> fullcmd[1];
    if (fullcmd[0] == "JOIN")
    {
        if (fullcmd[1].empty())
            sendMsgToClient(clientfd, "Plz, Provide a channnel name\n");
        else
        {
            Channel newchannel(fullcmd[1]);
            std::string msg = "Awosome, you just create " + newchannel.getChannelName() + " channel, Enjoy!\n";
            sendMsgToClient(clientfd, msg.c_str());
        }
    }
}