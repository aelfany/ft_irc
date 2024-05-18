#include "../../include/Channel.hpp"

Channel::Channel(std::string name)
{
    _name = name;
    _inv = false;
    _pass = false;
    _topc = false;
    _limit = -1;
    _usersSize = 0;
}

Channel::~Channel(){}

bool Channel::getPass()
{
    return _pass;
}

bool		Channel::checkUserexist(std::string _nickname) {
    map_users::iterator it = _users.begin();
    for(size_t a = 0; a < _users.size(); a++) {
        if(it->second.getNickName() == _nickname) {
            return true;
        }
    }
    return false;
}

bool            Channel::getTopc() {
    return _topc;
}

void	Channel::setUserLimit(bool limit)
{
    _userlimit = limit;
}
std::string&	Channel::getChannelName()
{
    return _name;
}

std::string Channel::getChannelNameDisplay()
{
    return _nameToDisplay;
}

void    Channel::setChannelNameDisplay(std::string _name)
{
    _nameToDisplay = _name;
}

std::string&	Channel::getPassword()
{
    return _password;
}

clientito&		Channel::getUserBynickname(std::string _nickname)
{
    map_users::iterator it = _users.begin();
    for(; it != _users.end(); it++) {
        if(it->second.getNickName() == _nickname)
            return it->second;
    }
    throw std::runtime_error("User not found");
}

bool	Channel::getPrvBynickname(std::string _nickname)
{
    map_users::iterator it = _users.begin();

    std::cout << _users.size() << std::endl;
    for(; it != _users.end(); it++) {
        if(it->second.getNickName() == _nickname)
        {
            // std::cout << it->first << std::endl;
            return it->first.first;
        }
    }
    throw std::runtime_error("bool not found");
}

void Channel::setPrvByNickname(std::string _nickname, bool prv, clientito & obj) {
    map_users::iterator it = _users.begin();
    std::pair<bool, std::string> p(prv, obj.getNickName());
    for(; it != _users.end(); it++) {
        if(it->second.getNickName() == _nickname) {
            _users.erase(it);
            _users.insert(std::make_pair(p, obj));
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

void    Channel::setinvited(int sockfd)
{
    _invited.push_back(sockfd);
}

bool	Channel::isInvited(int sockfd)
{
	std::vector<int>::iterator it = std::find(_invited.begin(), _invited.end(), sockfd);
    if(it != _invited.end())
    {
        _invited.erase(it);
        return true;
    }
    return false;
}

void Channel::pushtomap(bool privilege, clientito& obj)
{
    // clientito & t = getUserBynickname();
    std::pair<bool, std::string> p;
    p.first = privilege;
    p.second = obj.getNickName(); 
	_users.insert(std::make_pair(p, obj));
}
