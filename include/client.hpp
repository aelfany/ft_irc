#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

class clientito
{
     private:
        int	_sock_fd;
        clientito();
    public:
		clientito(int sockefd);
		~clientito();
		
		int getClinetFd();
};