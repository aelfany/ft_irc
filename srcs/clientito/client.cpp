#include "../../include/client.hpp"


clientito::clientito(int sockefd)
{
    _sock_fd = sockefd;
    _passFlag = false;
	_nickFlag = false;
	_userFlag = false;
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

bool	clientito::getpflag()
{
    return _passFlag;
}
bool	clientito::getnflag()
{
    return _nickFlag;
}
bool	clientito::getuflag()
{
    return _userFlag;
}

void 	clientito::setpflag(bool flag)
{
    _passFlag = flag;
}
void	clientito::setnflag(bool flag)
{
    _nickFlag = flag;
}
void	clientito::setuflag(bool flag)
{
    _userFlag = flag;
    _authenticated = true;
}
