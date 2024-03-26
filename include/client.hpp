#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

class clientito
{
     private:
        int		_sock_fd;
		bool	_passFlag;
		bool	_nickFlag;
		bool	_userFlag;
        bool	_authenticated;
        clientito();
    public:
		clientito(int sockefd);
		~clientito();
		
	int		getClinetFd();
    bool	isAuthed();
	bool	getpflag();
	bool	getnflag();
	bool	getuflag();

	void	setpflag();
	void	setnflag();
	void	setuflag();
};