#include "../../include/Server.hpp"

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

void	Servrr::setClientito(clientito& obj)
{
    this->_clients.push_back(obj);
}
