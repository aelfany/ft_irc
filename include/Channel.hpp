#pragma once

#include "client.hpp"
#include <map>

typedef std::map<std::string, clientito> map_users;

class Channel
{
    private:
        bool			_inv;
        bool			_pass;
        bool			_topc;
        bool            _userlimit;
        size_t          _limit;
        size_t			_usersSize;
        std::string		_name;
        std::string		_password;
        std::string		_topic;
        map_users		_users;
        std::string		_AdminName;
        Channel();
    public:
        Channel(std::string	_name);
        ~Channel();

		//getters
		std::string&	getChannelName();
		std::string&	getPassword();
        clientito&		getUserBynickname(std::string _nickname);
        map_users&		getUsersMap();
        bool            getPass();
        bool            getInvOnly();
		size_t			getusersSize();


		//setters
		void	setPassword(std::string	password);//
        void	setTopic(std::string	topic);
        void	setUserLimit(bool limit);
        void	setInvOnly(bool inv);
        void	setPass(bool pass);
        void	setTopc(bool top);
        void	setLimit(size_t limit);
		void	setusersSize(size_t user);

		void pushtomap(std::string _nickname, clientito& obj);
};

void ShowChannels(std::map<std::string, Channel> ChannelsMap);