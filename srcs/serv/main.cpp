#include "../../include/Server.hpp"


int main(int ac, char **av)
{
    programRequirement(ac, av[1]);
    //class Servvr takes a port number & a password
    Servrr server1(atoi(av[1]), av[2]);
    server1.setSockAddr();
    server1.runServer();

    struct pollfd poll_fd;
    poll_fd.fd = server1.getSockFd();
    poll_fd.events = POLLIN;
    std::vector<struct pollfd> &fds = server1.getPollfdVect();//declare vectore refrence with type struct pollfd 
    fds.push_back(poll_fd);

    size_t &i = server1._index;
    while(true)
    {
        int ret = poll(fds.data(), fds.size(), 1000);
        if (ret == -1)
        {
            perror("poll");
            exit(1);
        }
        // Check for events on each client socket
        for (i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == server1.getSockFd())
                    server1.eventOnServerSock();
                else
                    server1.eventOnClientSock();
            }
        }
    }
}
