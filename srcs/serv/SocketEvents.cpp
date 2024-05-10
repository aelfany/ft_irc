#include "../../include/Server.hpp"

void Servrr::eventOnServerSock()
{
    socklen_t client_addr_l = sizeof(_addr);
    int client_sock_fd = accept(getSockFd(), (struct sockaddr *)&_addr, &client_addr_l);
    if (client_sock_fd == -1)
        perror("accepttt");
    else
    {
        char cladd[16];
        inet_ntop(AF_INET, &_addr, cladd, 16);
        std::cout << "New connection from client with fd: " << client_sock_fd << " and address: " << cladd << std::endl;
        clientito cleintObj(client_sock_fd);
        setClientito(cleintObj);
        cleintObj.setIpAddr(cladd);
        struct pollfd poll_fd;
        poll_fd.fd = client_sock_fd;
        poll_fd.events = POLLIN | POLLOUT;
        getPollfdVect().push_back(poll_fd);
    }
}


void Servrr::eraseChannel(std::string _name)
{
    _channels.erase(_name);
}

void Servrr::removeFromChannel(int client_fd)
{
    channelsMap::iterator it = _channels.begin();

    for (; it != _channels.end(); it++)
    {
        map_users &users_map = it->second.getUsersMap();
        map_users::iterator iter = users_map.begin();
        for (;iter != users_map.end(); ++iter)
        {
            if(client_fd == iter->second.getClinetFd())
            {
                broadcastMessage(it->second, ":" + iter->second.getNickName() + "!~" + iter->second.getNickName() + "@127.0.0.1 QUIT :Remote host closed the connection\r\n", client_fd);
                users_map.erase(iter);
                it->second.setusersSize(-1);
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
    clientito &clientobj = getClientitoByfd(client_sock_fd);
    char buffer[1024];
    memset(buffer, 0, 1024);
    ssize_t recvData = recv(client_sock_fd, buffer, sizeof(buffer), 0);
    if (recvData == -1)
        perror("recvvv");
    else if (recvData == 0)
    {
        return  ;
    }
    else
    {
        if (strstr(buffer, "\r\n"))
        {
            std::cout << "Received1: " << buffer << std::endl;
            command(buffer, _index);
            memset(buffer, 0, 1024);
        }
        else
        {
            std::cout << "Received2: " << buffer << std::endl;
            std::string _line;
            _line = _line.append(buffer);
            memset(buffer, 0, 1024);
            clientobj.setrecvLine(_line);
            command(clientobj.getrecvLine(), _index);
        }
    }
     // std::cout << "handle CTR+D for client with sfd: " << _clients[_index-1].getClinetFd() << std::endl;
    // else
    // {
    //     std::cout << "Received: " << buffer << std::endl;
    //     command(buffer, _index);
    // }
}