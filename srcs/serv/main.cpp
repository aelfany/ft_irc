#include "../../include/Server.hpp"

int main(int ac, char **av)
{
    struct pollfd poll_fd;
    struct sockaddr_in addr;
    std::vector<struct pollfd> fds;//declare vectore with type struct pollfd

    simpleRules(ac, av[1]);
    int portN = atoi(av[1]);

    //class Servvr takes a port number & a password
    Servrr server1(portN, av[2]);
    SocketAddrInfo(addr, portN);
    server1.runServer(addr);
    interFace(server1);

    poll_fd.fd = server1.getSockFd();//sock server fd
    poll_fd.events = POLLIN;//1

    // Set up pollfd vector
    fds.push_back(poll_fd);
    socklen_t client_addr_l = sizeof(addr);
    while(true)
    {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
        {
            perror("poll");
            exit(1);
        }
        // Check for events on each client socket
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                // Event on server socket (new connection)
                if (fds[i].fd == server1.getSockFd())
                {
                    int client_sock_fd = accept(server1.getSockFd(), (struct sockaddr *)&addr, &client_addr_l);
                    if (client_sock_fd == -1)
                        perror("accept");
                    else
                    {
                        struct pollfd poll_fd;
                        poll_fd.fd = client_sock_fd;
                        poll_fd.events = POLLIN;
                        fds.push_back(poll_fd);
                        std::cout << "New connection from client with fd: " << client_sock_fd << std::endl;
                    }
                }
                else
                {
                    // Event on client socket (data available)
                    int client_sock_fd = fds[i].fd;
                    char buffer[1024];
                    ssize_t bytesRead = recv(client_sock_fd, buffer, sizeof(buffer), 0);
                    if (bytesRead == -1)
                        perror("recv");
                    else if (bytesRead == 0)
                    {
                        // If client disconnected print this==> && close its socket && erase its data in our vector
                        std::cout << "Client "<< client_sock_fd << " disconnected" << std::endl;
                        close(client_sock_fd);
                        fds.erase(fds.begin() + i);
                        i--;
                    }
                    else
                        std::cout << "Received: " << std::string(buffer, bytesRead) << std::endl;
                }
            }
        }
    }
}