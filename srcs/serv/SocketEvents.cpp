#include "../../include/Server.hpp"

void Servrr::eventOnServerSock()
{
    socklen_t client_addr_l = sizeof(_addr);
    int client_sock_fd = accept(getSockFd(), (struct sockaddr *)&_addr, &client_addr_l);
    if (client_sock_fd == -1)
        perror("accepttt");
    else
    {
        std::cout << "New connection from client with fd: " << client_sock_fd << std::endl;
        clientito cleintObj(client_sock_fd);
        setClientito(cleintObj);
        struct pollfd poll_fd;
        poll_fd.fd = client_sock_fd;
        poll_fd.events = POLLIN | POLLOUT;
        getPollfdVect().push_back(poll_fd);
    }
}



void Servrr::removeFromChannel(int client_fd)
{
    channelsMap::iterator it = _channels.begin();

    for (; it != _channels.end(); it++)
    {
        map_users &users_map = it->second.getUsersMap();
        map_users::iterator iter = users_map.begin();
        for (;iter != users_map.end(); iter++)
        {
            if(client_fd == iter->second.getClinetFd())
            {
                broadcastMessage(it->second, ":" + iter->second.getNickName() + "!~" + iter->second.getNickName() + "@127.0.0.1 QUIT :Remote host closed the connection\r\n", client_fd);
                users_map.erase(iter);
                break ;
            }
        }
    }
}

void Servrr::eventOnClientSock()
{
    std::vector<pollfd>& fds = getPollfdVect();
    // Event on client socket (data available)
    int client_sock_fd = fds[_index].fd;
    char buffer[1024];
    memset(buffer, 0, 1024);
    ssize_t recvData = recv(client_sock_fd, buffer, sizeof(buffer), 0);
    if (recvData == -1)
        perror("recvvv");
    else if (recvData == 0)
    {
        // If client disconnected print this==> && close its socket && erase its data in our vector
        std::cout << "Client "<< client_sock_fd << " disconnected" << std::endl;
        close(client_sock_fd);
        fds.erase(fds.begin() + _index);
        removeClient(_index-1);
        //remove client from channel
        removeFromChannel(client_sock_fd);
        //send message to channels he joined that client has been deconnected
        _index--;
    }
    else
    {
        std::cout << "Received: " << buffer << std::endl;
        command(buffer, _index);
    }
}