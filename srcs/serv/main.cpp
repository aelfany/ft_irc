#include "../../include/Server.hpp"

bool check_port(std::string port)
{
    if (port[0] == '+' || port[0] == '-')
        return false;
    for(size_t i = 0; i < port.length(); ++i)
    {
        if (!isdigit(port[i]))
            return false;
    }
    return true;
}

void simpleRules(int ac, char *port)
{
    if(ac != 3)
    {
        std::cerr << "ERROR: <.ircserv> <port> <password>\n";
        exit(EXIT_FAILURE);
    }
    if (!check_port(port))
    {
        std::cerr << "ERROR: '" << port << "' : check port number then try again...\n";
        exit(EXIT_FAILURE);
    }
}

void SocketAddrInfo(sockaddr_in& addr, char *port_n)
{
    int port_number = atoi(port_n);
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);// Accept connections on any interface
    addr.sin_port = htons(port_number);// Port number (use htons to convert to network byte order)
}

int main(int ac, char **av)
{
    simpleRules(ac, av[1]);
    struct sockaddr_in addr;
    SocketAddrInfo(addr, av[1]);
    Servrr server1(ntohs(addr.sin_port), av[2]);
    server1.setSockAddr(addr);
    
    while(true)
    {
        std::cout << server1.getPort() << std::endl;
        std::cout << server1.getPassword() << std::endl;
        std::cout << server1.getSockFd() << std::endl;
        exit(1);
    }

}