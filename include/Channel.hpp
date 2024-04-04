#pragma once

#include "client.hpp"
#include <map>

typedef std::map<std::string, clientito> map_users;

class Channel
{
    private:
        // int			_id;
        std::string	_name;
        std::string	_password;
        map_users	_users;
        Channel();
    public:
        Channel(std::string	_name);
        ~Channel();

		//getters
		std::string&	getChannelName();
		std::string&	getPassword();
		clientito&		getUser(std::string _nickname);

		//setters
		void	setPassword(std::string	password);

		void pushtomap(std::string _nickname, clientito& obj);
};