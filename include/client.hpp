#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

class clientito
{
     private:
        int			_sock_fd;
		bool		_passFlag;
		bool		_nickFlag;
		bool		_userFlag;
        bool		_authenticated;
		bool		_operator;
	 	std::string _nickName;
    public:
        clientito();
		clientito(int sockefd);
		~clientito();
	//getters
	std::string	getNickName();
	int			getClinetFd();
    bool		isAuthed();
	bool		getpflag();
	bool		getnflag();
	bool		getuflag();
	bool		getOperator();
	//setters
	void 	setNickName(std::string nickname);
	void	setpflag(bool flag);
	void	setnflag(bool flag);
	void	setuflag(bool flag);
	void	setOperator(bool flag);
};
