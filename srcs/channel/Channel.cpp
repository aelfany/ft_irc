#include "../../include/Channel.hpp"

Channel::Channel(std::string    name)
{
    _name = name;
}

Channel::~Channel(){}

std::string&	Channel::getChannelName()
{
    return _name;
}

std::string&	Channel::getPassword()
{
    return _password;
}

clientito&		Channel::getUser(std::string _nickname)
{
    return _users[_nickname];
}

void	Channel::setPassword(std::string	password)
{
    _password = password;
}

void    Channel::pushtomap(std::string _nickname, clientito& obj)
{
	_users.insert(std::make_pair(_nickname, obj));
}
