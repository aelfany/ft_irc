#include "../../include/Channel.hpp"

Channel::Channel(std::string name)
{
    _name = name;
    _inv = false;
    _pass = false;
    _topc = false;
    _limit = -1;
    _usersSize = 1;
}

Channel::~Channel(){}

bool Channel::getPass()
{
    return _pass;
}

void	Channel::setUserLimit(bool limit)
{
    _userlimit = limit;
}
std::string&	Channel::getChannelName()
{
    return _name;
}

std::string&	Channel::getPassword()
{
    return _password;
}

clientito&		Channel::getUserBynickname(std::string _nickname)
{
    return _users[_nickname];
}

map_users&		Channel::getUsersMap()
{
	return _users;
}

void	Channel::setPassword(std::string password)
{
    _password = password;
}

bool    Channel::getInvOnly()
{
    return _inv;
}

size_t  Channel::getusersSize()
{
    return _usersSize;
}

void	Channel::setTopic(std::string topic)
{
    _topic = topic;
}

void	Channel::setInvOnly(bool inv)
{
    _inv = inv;
}
void	Channel::setPass(bool pass)
{
    _pass = pass;
}
void	Channel::setTopc(bool top)
{
    _topc = top;
}
void	Channel::setLimit(size_t limit)
{
    _limit = limit;
}

void	Channel::setusersSize(size_t user)
{
    _usersSize += user;
}

void Channel::pushtomap(std::string _nickname, clientito& obj)
{
	_users.insert(std::make_pair(_nickname, obj));
}
