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

class Servrr
{
    private:
		//NOT USED YET.
        typedef  void (Servrr::*ptmf)(bool);//define a pointer to member function type.
        ptmf _ptrs[3];//declare three ptrs here.(shoof constructur)

        int				_portNumber;
        sockaddr_in		_addr;
        int				_sock_fd;
        std::string		_password;
        std::string		args[2];
        std::vector<clientito>	_clients;
    public:
        bool pass_flag, nick_flag, user_flag;
        Servrr() : pass_flag(false), nick_flag(false), user_flag(false) {};
        void  auth(char *str, int client_sock_fd);
        void trimSpaces(const std::string& str);
        Servrr(int portNumber, std::string passw);
        ~Servrr();

		std::string&	getPassword();
		unsigned int	getPort();
		int				getSockFd();
		struct sockaddr_in&	getSockAddr();

		void	setSockAddr(sockaddr_in	addr);
		void	setSockFd(int sock_fd);
		void	setClientito(clientito obj);
        clientito&	getClientito(int id);
        void runServer(struct sockaddr_in& addr);

        //checkers //NOT USED YET.
        // void checkPass(bool t);
        // void checkNick(bool t);
        // void checkUser(bool t);
};


void sendMsgToClient(int clientfd, const char msg[]);
bool check_port(std::string port);
void simpleRules(int ac, char *port);
void SocketAddrInfo(struct sockaddr_in& addr, int port_n);
void interFace(Servrr obj);