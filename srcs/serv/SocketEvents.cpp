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
        _index--;
    }
    else
    {
        std::cout << "Received: " << buffer << std::endl;
        command(buffer, _index);
    }
}