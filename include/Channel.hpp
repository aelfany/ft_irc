#pragma once

#include "client.hpp"
#include <map>
#include <vector>

// typedef std::map<std::string, clientito> map_users;
typedef std::map<std::pair<bool, int>, clientito> map_users;
//                     is_opertator==1  ,obj

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
        std::string		topicseter;
        std::string		topictime;
        std::string		_password;
        std::string		_topic;
        map_users		_users;
        std::string		_nameToDisplay;
        std::vector<int> _invited;
    public:
        void updateNickname(std::string _nickname, std::string oldnick, bool prv, clientito & obj);
        Channel(std::string	_name);
        ~Channel();
        Channel(){};
        void            setPrvByNickname(std::string _nickname, bool prv);
		//getters
        void            deletClient(std::string nick);
		std::string&	getChannelName();
        std::string     getChannelNameDisplay();
        void            setChannelNameDisplay(std::string _name);
		std::string&	getPassword();
        void            setTopicAttr(std::string topic, bool topc, std::string seter, std::string time);
        clientito&		getUserBynickname(std::string _nickname);
        bool	        getPrvBynickname(std::string _nickname);
        map_users&		getUsersMap();
        bool            getPass();
        bool            getInvOnly();
        bool            getTopc();
		size_t			getusersSize();
        size_t          getlimit();
        bool	        getUserLimit();
        bool            isInvited(int sockfd);


		//setters
		void	setPassword(std::string	password);//
        void	setTopic(std::string	topic);
        void	setUserLimit(bool limit);
        void	setInvOnly(bool inv);
        void	setPass(bool pass);
        void	setTopc(bool top);
        void	setLimit(size_t limit);
		void	setusersSize(size_t user);
        void    setinvited(int sockfd);
        bool    checkUserexist(std::string _nickname);
        std::string     gettopicseter();
        std::string     getTopic();
        std::string     gettopictime();

		void 	pushtomap(bool privilege, clientito& obj);
       	void	removeInvitedClient(int fd);
};

void ShowChannels(std::map<std::string, Channel> ChannelsMap);