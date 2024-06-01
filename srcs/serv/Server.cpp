#include "../../include/Server.hpp"
#include <fcntl.h>
#include <poll.h>

Servrr::Servrr(int portNumber, std::string passw)
{
    _portNumber = portNumber;
    _password = passw;
    _sock_fd = 0;
    _index = 0;
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

struct sockaddr_in& Servrr::getSockAddr()
{
    return _addr;
}

clientito&	Servrr::getClientitoByIndex(int id)
{
    return _clients[id];
}

clientito&	Servrr::getClientitoByfd(int clientfd)
{
    for(size_t i = 0; i < _clients.size(); i++)
    {
		if (_clients[i].getClinetFd() == clientfd)
			return _clients[i];
    }
    return _clients[0];//don't forget to remove this, and change it with a proper way.
}


clientito&	Servrr::getClient(std::string nick)
{
    clientVect::iterator it = _clients.begin();
    for(; it != _clients.end(); it++) {
		if (it->getNickName() == nick)
			return *it;
    }
    throw "no user found";
}


void    Servrr::removeClient(int id)
{
    _clients.erase(_clients.begin() +id);
}


void    Servrr::setSockAddr()
{
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = PF_INET;
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);// Accept connections on any interface 
    _addr.sin_port = htons(_portNumber);// Port number (use htons to convert to network byte order)
}

void    Servrr::setSockFd(int sock_fd)
{
    _sock_fd = sock_fd;
}

PollfdVect&	Servrr::getPollfdVect()
{
	return _fds;
}

void	Servrr::setClientito(clientito obj)
{
    this->_clients.push_back(obj);
}


void Servrr::runServer()
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
        close(_sock_fd);
        exit(EXIT_FAILURE);
    }
    int valueOfOpt = 1;
     //setsockopt() is used to specify some options for the socket to control the behavior of the socket.
    if (setsockopt(_sock_fd, SOL_SOCKET, SO_REUSEADDR, &valueOfOpt, sizeof(valueOfOpt)) == -1)
    {
        std::cerr <<"Error: setsockopt() failed\n";
        close(_sock_fd);
        exit(1);
    }
    if (bind(_sock_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        std::cerr <<"Error: bind() failed\n";
        close(_sock_fd);
        exit(1);
    }
    if (listen(_sock_fd, 10) == -1)
    {
        std::cerr <<"Error: listen() failed\n";
        close(_sock_fd);
        exit(1);
    }
    interFace(*this);

}

int check_port(std::string port, std::string pass)
{
    std::stringstream  s(port);
    int p;
    s >> p;
    if (p < 1024 || p > 65535 || port.empty())
        return 1;
    if(pass.empty())
        return 2;
    if (port[0] == '+' || port[0] == '-')
        return 1;
    for(size_t i = 0; i < port.length(); ++i)
    {
        if (!isdigit(port[i]))
            return 1;
    }
    return 0;
}

void programRequirement(int ac, char *port, char *pass)
{
    if(ac != 3)
    {
        std::cerr << "ERROR: <.ircserv> <port> <password>\n";
        exit(EXIT_FAILURE);
    }
    if (check_port(port, pass) != 0)
    {
        if (check_port(port, pass) == 2)
            std::cerr << "ERROR: '" << pass << "' : check pass then try again...\n";
        else
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

void sendMsgToClient(int clientfd, std::string msg)
{
    if (!msg.empty())
    {
        ssize_t bytes = send(clientfd, msg.c_str(), msg.size(), 0);
        if (bytes == -1)
            perror("send: ");
    }
}