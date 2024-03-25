#include "../../include/Server.hpp"
#include <fcntl.h>
#include <poll.h>

Servrr::Servrr(int portNumber, std::string passw)
{
    _portNumber = portNumber;
    _password = passw;
    _sock_fd = 0;
}

Servrr::~Servrr()
{
}

std::string&	Servrr::getPassword()
{
    return _password;
}
unsigned int	Servrr::getPort()
{
    return _portNumber;
}
int	Servrr::getSockFd()
{
    return _sock_fd;
}

void    Servrr::setSockAddr(sockaddr_in	addr)
{
    _addr = addr;
}

void    Servrr::setSockFd(int sock_fd)
{
    _sock_fd = sock_fd;
}

void	Servrr::setClientito(clientito obj)
{
    this->_clients.push_back(obj);
}

std::vector<clientito>	Servrr::getClientito()
{
    return _clients;
}

void Servrr::runServer(struct sockaddr_in& addr)
{
    //create a socket
    _sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(_sock_fd == -1)
    {
        std::cerr <<"Error:  failed to create a socket\n";
            exit(1);
    }
    //Set server socket to non-blocking mode
    if (fcntl(_sock_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    int valueOfOpt = 1;
     //setsockopt() is used to specify some options for the socket to control the behavior of the socket.
    if (setsockopt(_sock_fd, SOL_SOCKET, SO_REUSEADDR, &valueOfOpt, sizeof(valueOfOpt)) == -1)
    {
        std::cerr <<"Error: setsockopt() failed\n";
        exit(1);
    }
    if (bind(_sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cerr <<"Error: bind() failed\n";
        exit(1);
    }
    if (listen(_sock_fd, 10) == -1)
    {
        std::cerr <<"Error: listen() failed\n";
        exit(1);
    }

}

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

void SocketAddrInfo(struct sockaddr_in& addr, int port_n)
{
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);// Accept connections on any interface
    addr.sin_port = htons(port_n);// Port number (use htons to convert to network byte order)
}

void interFace(Servrr obj)
{
    //print some basic info
    std::cout << "#---------------------------#" << std::endl;
    std::cout << "#  Port    : "<< obj.getPort()<< std::endl;
    std::cout << "#  Password: "<< obj.getPassword()<< std::endl;
    std::cout << "#---------------------------#" << std::endl;
}

void sendMsgToClient(int clientfd, const char* msg)
{
    if (msg != NULL)
    {
    std::cout << "msg: " << msg << std::endl;
        ssize_t bytes = send(clientfd, msg, sizeof(msg), 0);
        if (bytes == -1)
            perror("send: ");
    }
}