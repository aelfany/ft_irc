#include "../../include/Server.hpp"

void Servrr::eventOnServerSock()
{
    socklen_t client_addr_l = sizeof(_addr);
    int client_sock_fd = accept(getSockFd(), (struct sockaddr *)&_addr, &client_addr_l);
    std::string clip = inet_ntoa(_addr.sin_addr);
    if (client_sock_fd == -1)
        perror("accepttt");
    else
    {
        std::cout << "New connection from client with fd: " << client_sock_fd << " and IP: " << clip << std::endl;
        clientito cleintObj(client_sock_fd);
        cleintObj.setipaddr(clip);
        setClientito(cleintObj);
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
                
                broadcastMessage(it->second, RPL_QUIT(iter->second.getNickName(), host, "good bye"), client_fd);
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
    buffer[recvData] = '\0';
    if (recvData == -1)
        perror("recv");
    else if (recvData == 0)
    {
            // If client disconnected print this==> && close its socket && erase its data in our vector
            std::cout << "Client "<< client_sock_fd << " disconnected" << std::endl;
            close(client_sock_fd);
            _fds.erase(_fds.begin() + _index);
            removeClient(_index-1);
            //remove client from channel && send message to channels he joined that client has been deconnected
            removeFromChannel(client_sock_fd);
            _index--;
            channelsMap::iterator it = _channels.begin();
            while (it != _channels.end())
            {
                if (it->second.getusersSize() == 0)
                    it = _channels.erase(it);
                else
                    ++it;
            }
        return  ;
    }
    else
    {
        clientobj.setrecvLine(buffer);// join a join b \r\n
            memset(buffer, 0, 1024);
        if (strstr(clientobj.getrecvLine().c_str(), "\n"))
        {
            size_t pos  = clientobj.getrecvLine().find_first_of("\n");

            if (pos > clientobj.getrecvLine().size())
                return;
            std::string cmd = clientobj.getrecvLine().substr(0, pos+1);
            std::cout << "\033[0;31m" << cmd << "\033[0m" << std::endl;
            command(cmd, _index);
            clientobj.getrecvLine() = clientobj.getrecvLine().erase(0, pos+1);
        }
    }
}