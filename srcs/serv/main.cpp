#include "../../include/Server.hpp"



void handle_sigpipe(int signum) {
    std::cout << "Received SIGPIPE signal, ignoring it." << std::endl;
    (void)signum;
}

int main(int ac, char **av)
{
        signal(SIGPIPE, handle_sigpipe);
        programRequirement(ac, av[1], av[2]);
        Servrr server1(atoi(av[1]), av[2]);
        server1.setSockAddr();
        server1.runServer();

        struct pollfd poll_fd;
        poll_fd.fd = server1.getSockFd();
        poll_fd.events = POLLIN;
        std::vector<struct pollfd> &fds = server1.getPollfdVect();//declare vectore refrence with type struct pollfd 
        fds.push_back(poll_fd);
        size_t &i = server1._index;
        while (true) {
            int ret = poll(fds.data(), fds.size(), -1);
            if (ret == -1) {
                if (errno == EINTR) {
                    continue;
                }
                perror("poll");
                exit(1);
            }
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
