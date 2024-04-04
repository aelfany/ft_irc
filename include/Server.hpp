#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.hpp"
#include "Channel.hpp"
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <netinet/in.h>
#include <sstream>
#include <map>

#define WELCOMING "--------------------welcome dear client!--------------------\nNote: please Enter PASS, NICK, and USER for authenticating\n\t\tFollow this form: CMD example\n\n";
#define AUTHED "\n---------------------------------------------------------------\n\tAuthentication success, Welcome again, BIG DOG!\n---------------------------------------------------------------\n";

typedef std::vector<clientito> clientVect;//vector of clients
typedef std::map<std::string, Channel> channelsMap;//map of channels
typedef std::vector<std::string> JoinVect;//vector for Parse join
class Servrr
{
    private:
    
            int							_portNumber;
        sockaddr_in					_addr;
        int							_sock_fd;
        std::string					_password;
        std::vector<std::string>         args;
        clientVect	                _clients;
        channelsMap					_channels;
        JoinVect					_result;
        Servrr();
    public:
        void command(std::string buffer, size_t i, Servrr& servrr);
        void parsNick(clientito& client);
        // void parsUser(clientito& client);
        Servrr(int portNumber, std::string passw);
        ~Servrr();

		void	auth2(std::string str, clientito& client);
        void    trimUser(const std::string& str);
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

        //channel section
        void	createChannel(std::string command, int client_fd);
		void	parseJoinCommand(const std::string& command);
		void	proccessChannels(int clientfd);
};

void	sendMsgToClient(int clientfd, std::string msg);
bool	check_port(std::string port);
void	programRequirement(int ac, char *port);
void	SocketAddrInfo(struct sockaddr_in& addr, int port_n);
void	interFace(Servrr obj);