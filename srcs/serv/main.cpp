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
    //class Servvr takes a port number & a password
    Servrr server1(ntohs(addr.sin_port), av[2]);
    //setSockAddr() is a setter that gonna set the struct named addr
    server1.setSockAddr(addr);
    
    //print some basic info
    std::cout << server1.getPort() << std::endl;
    std::cout << server1.getPassword() << std::endl;
    std::cout << server1.getSockFd() << std::endl;

    //create a socket
    server1.setSockFd(socket(PF_INET, SOCK_STREAM, 0));
    if(server1.getSockFd() == -1)
    {
        std::cerr <<"Error:  failed to create a socket\n";
            exit(1);
    }
    int valueOfOpt = 1;
     //setsockopt() is used to specify some options for the socket to control the behavior of the socket.
    if (setsockopt(server1.getSockFd(), SOL_SOCKET, SO_REUSEADDR, &valueOfOpt, sizeof(valueOfOpt)) == -1)
    {
        std::cerr <<"Error: setsockopt() failed\n";
        exit(1);
    }
    if (bind(server1.getSockFd(), (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cerr <<"Error: bind() failed\n";
        exit(1);
    }
    if (listen(server1.getSockFd(), 10) == -1)
    {
        std::cerr <<"Error: listen() failed\n";
        exit(1);
    }
    //loop over, and listening on incomming connections or data.
    socklen_t client_addr_l = sizeof(addr);
    while(true)
    {
        int client_sock_fd = accept(server1.getSockFd(), (struct sockaddr *)&addr, &client_addr_l);
        if (client_sock_fd != -1)
        {
            clientito client_obj(client_sock_fd);
            server1.setClientito(client_obj);
        }
        std::cout << "server is lestining on the above port\n" << std::endl;
    }

}