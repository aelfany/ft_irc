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

bool		Channel::checkUserexist(std::string _nickname)
{
    map_users::iterator it = _users.begin();
    for(; it != _users.end(); it++) {
        if(it->second.getNickName() == _nickname)
            return true;
    }
    return false;
}

bool	Channel::getPrvBynickname(std::string _nickname)
{
    map_users::iterator it = _users.begin();

    std::cout << _users.size() << std::endl;
    for(; it != _users.end(); it++) {
        if(it->second.getNickName() == _nickname)
        {
            std::cout << it->first << std::endl;
            return it->first;
        }
    }
    throw std::runtime_error("bool not found");
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

void Channel::pushtomap(bool privilege, clientito& obj)
{
    // clientito & t = getUserBynickname();
	_users.insert(std::make_pair(privilege, obj));
}
