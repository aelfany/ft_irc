#include "../../include/client.hpp"


clientito::clientito(int sockefd)
{
    _sock_fd = sockefd;
    _authenticated = false;
}

clientito::~clientito(){}

int clientito::getClinetFd()
{
    return _sock_fd;
}

bool clientito::isAuthed()
{
    return _authenticated;
}
