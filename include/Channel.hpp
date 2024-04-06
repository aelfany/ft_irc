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
        size_t          _limit;
        std::string		_name;
        std::string		_password;
        std::string		_topic;
        map_users		_users;
        Channel();
    public:
        Channel(std::string	_name);
        ~Channel();

		//getters
		std::string&	getChannelName();
		std::string&	getPassword();
		clientito&		getUser(std::string _nickname);

		//setters
		void	setPassword(std::string	password);//
        void	setTopic(std::string	topic);

        void	setInvOnly(bool inv);
        void	setPass(bool pass);
        void	setTopc(bool top);
        void	setLimit(size_t limit);

		void pushtomap(std::string _nickname, clientito& obj);
};