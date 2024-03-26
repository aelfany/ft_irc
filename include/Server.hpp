#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.hpp"
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <netinet/in.h>
#include <sstream>

#define WELCOMING "--------------------welcome dear client!--------------------\nNote: please Enter PASS, NICK, and USER for authenticating\n\t\tFollow this form: CMD example\n\n";
#define AUTHED "\n---------------------------------------------------------------\n\tAuthentication success, Welcome again, BIG DOG!\n---------------------------------------------------------------\n";

typedef std::vector<clientito> clientVect;

class Servrr
{
    private:
        int							_portNumber;
        sockaddr_in					_addr;
        int							_sock_fd;
        std::string					_password;
        std::string					args[2];
        clientVect	                _clients;
        Servrr();
    public:
        Servrr(int portNumber, std::string passw);
        ~Servrr();

		void	auth2(char *str, clientito& client);
        void	trimSpaces(const std::string& str);

		std::string&		getPassword();
		unsigned int		getPort();
		int					getSockFd();
		struct sockaddr_in&	getSockAddr();
        clientito&			getClientito(int id);

		void	setSockAddr(sockaddr_in	addr);
		void	setSockFd(int sock_fd);
		void	setClientito(clientito obj);

        void	runServer(struct sockaddr_in& addr);
        void	removeClient(int id);
};

void	sendMsgToClient(int clientfd, const char msg[]);
bool	check_port(std::string port);
void	programRequirement(int ac, char *port);
void	SocketAddrInfo(struct sockaddr_in& addr, int port_n);
void	interFace(Servrr obj);