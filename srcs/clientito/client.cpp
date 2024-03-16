#include "../../include/client.hpp"


clientito::clientito(int sockefd)
{
    _sock_fd = sockefd;
}

clientito::~clientito(){}

int clientito::getClinetFd()
{
    return _sock_fd;
}
