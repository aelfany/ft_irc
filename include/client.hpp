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
	 	std::string _nickName;
    public:
        clientito();
		clientito(int sockefd);
		~clientito();
		
	std::string 	getNickName();
	int		getClinetFd();
    bool	isAuthed();
	bool	getpflag();
	bool	getnflag();
	bool	getuflag();

	void 	setNickName(std::string nickname);
	void	setpflag(bool flag);
	void	setnflag(bool flag);
	void	setuflag(bool flag);
};
