#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.hpp"
#include <vector>

class Servrr
{
    private:
        int				_portNumber;
        sockaddr_in		_addr;
        int				_sock_fd;
        std::string		_password;
		std::vector<clientito> _clients;
        Servrr();
    public:
        Servrr(int portNumber, std::string passw);
        ~Servrr();

		std::string&	getPassword();
		unsigned int	getPort();
		int				getSockFd();

		void	setSockAddr(sockaddr_in	addr);
		void	setSockFd(int sock_fd);
		void	setClientito(clientito& obj);
       std::vector<clientito>	getClientito();
};