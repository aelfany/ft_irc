#include "../../include/client.hpp"

 clientito::clientito()
 {
    _passFlag = false;
	_nickFlag = false;
	_userFlag = false;
    _authenticated = false;
    _operator = false;
 }

clientito::clientito(int sockefd)
{
    _sock_fd = sockefd;
    _passFlag = false;
	_nickFlag = false;
	_userFlag = false;
    _authenticated = false;
    _operator = false;
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

std::string clientito::getRealName() {
    return _RealName;
}
std::string clientito::getUserName() {
    return _UserName;
}
void 		clientito::setRealName(std::string realname) {
    _RealName = realname;
}
void 		clientito::setUserName(std::string username) {
    _UserName = username;
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

bool	clientito::getOperator()
{
	return _operator;
}

std::string& clientito::getipaddr()
{
    return _ipAddr;
}

void 	clientito::setNickName(std::string nickname) {
    _nickName = nickname;
}
std::string 	clientito::getNickName() {
    return _nickName;
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

void	clientito::setOperator(bool oper)
{
	_operator = oper;
}

std::string& clientito::getrecvLine()
{
    return _recvLine;
}

void    clientito::setrecvLine(std::string recvline)
{
    _recvLine += recvline;
}

void    clientito::setipaddr(std::string _ip)
{
    _ipAddr = _ip;
}
