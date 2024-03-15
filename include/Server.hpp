#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

class Servrr
{
    private:
        int _portNumber;
        sockaddr_in		_addr;
        int				_sock_fd;
        std::string		_password;
        Servrr();
    public:
        Servrr(int portNumber, std::string passw);
        ~Servrr();

		std::string&	getPassword();
		unsigned int	getPort();
		int				getSockFd();
		void			setSockAddr(sockaddr_in	addr);
};