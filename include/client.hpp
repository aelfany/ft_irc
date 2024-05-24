#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

class clientito
{
     private:
        int			_sock_fd;
		std::string	_ipAddr;//added_recently(09/May)
		bool		_passFlag;
		bool		_nickFlag;
		bool		_userFlag;
        bool		_authenticated;
		bool		_operator;
		std::string _UserName;
	 	std::string _RealName;
	 	std::string _nickName;
		std::string _recvLine;
		std::vector<std::string> channels;
    public:
        clientito();
		clientito(int sockefd);
		~clientito();
	//getters
	void    	pushChannel(std::string channel);
	std::vector<std::string> getChannels();
	std::string getRealName();
	std::string getUserName();
	std::string	getNickName();
	int			getClinetFd();
    bool		isAuthed();
	bool		getpflag();
	bool		getnflag();
	bool		getuflag();
	bool		getOperator();
	std::string& getrecvLine();
	std::string& getipaddr();

	//setters
	void 	setRealName(std::string realname);
	void 	setUserName(std::string username);
	void 	setNickName(std::string nickname);
	void	setpflag(bool flag);
	void	setnflag(bool flag);
	void	setuflag(bool flag);
	void	setOperator(bool flag);
	void	setipaddr(std::string _ip);

	void setrecvLine(std::string recvline);
};
