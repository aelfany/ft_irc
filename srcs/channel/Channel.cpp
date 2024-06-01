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
    for(;it != _users.end(); it++) {
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

bool	Channel::getUserLimit()
{
    return _userlimit;
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
        if(it->second.getNickName() == _nickname) {
            return it->second;
        }
    }
    throw "User not found";
}

bool	Channel::getPrvBynickname(std::string _nickname)
{
    map_users::iterator it = _users.begin();

    for(; it != _users.end(); it++) {
        if(it->second.getNickName() == _nickname)
            return it->first.first;
    }
    throw "bool not found";
}

void Channel::setPrvByNickname(std::string _nickname, bool prv) {
    
    std::map<std::pair<bool, int>, clientito>::iterator it = _users.begin();
    bool found = false;
    for (; it != _users.end(); ++it) {
        if (it->second.getNickName() == _nickname) {
            clientito tempObj = it->second;
            int x = it->second.getClinetFd();
            std::pair<bool, int> newKey(prv, x);
            _users.insert(std::make_pair(newKey, tempObj));
            _users.erase(it);
            found = true;
            break; 
        }
    }
    if (!found) {
        throw "Nickname not found";
    }
    for (std::map<std::pair<bool, int>, clientito>::iterator it2 = _users.begin(); it2 != _users.end(); ++it2) {
        std::cout << "\033[0;31m" << "##################" << "\033[0m" << std::endl;
        std::cout << it2->second.getNickName() << std::endl;
        std::cout << "\033[0;31m" << "##################" << "\033[0m" << std::endl;
    }
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

void    clientito::pushChannel(std::string channel) {
    channels.push_back(channel);
}

size_t  Channel::getlimit()
{
    return _limit;
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
        // _invited.erase(it);
        return true;
    }
    return false;
}

void	Channel::removeInvitedClient(int sockfd)
{
    std::vector<int>::iterator it = std::find(_invited.begin(), _invited.end(), sockfd);
    if(it != _invited.end())
        _invited.erase(it);
}

void Channel::pushtomap(bool privilege, clientito& obj)
{
    std::pair<bool, int> p(privilege, obj.getClinetFd());
	_users.insert(std::make_pair(p, obj));
}
