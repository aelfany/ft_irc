#pragma once

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.hpp"
#include "replies.hpp"
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
typedef std::vector<struct pollfd> PollfdVect;

class Servrr
{
    private:
        int							_portNumber;
        std::string					_password;
        PollfdVect					_fds;
        sockaddr_in					_addr;
        int							_sock_fd;
        clientVect	                _clients;
        channelsMap					_channels;
        JoinVect					_result;
        std::vector<std::string>    args;
        Servrr();
    public:
        size_t	_index;
        void command(std::string buffer, size_t i);
        Servrr(int portNumber, std::string passw);
        ~Servrr();
        void parsNick(clientito& client);
        // void parsUser(clientito& client);

		void	auth2(std::string str, clientito& client);
        void    trimUser(const std::string& str);
        Channel & getChannel(std::string channel);
        void    trimSpaces(const std::string& str, bool x);

		std::string&		getPassword();
		unsigned int		getPort();
		int					getSockFd();
		PollfdVect&			getPollfdVect();
		struct sockaddr_in&	getSockAddr();
        clientito&			getClientitoByIndex(int index);
        clientito&			getClientitoByfd(int clientfd);

		void	setSockAddr();
		void	setSockFd(int sock_fd);
		void	setClientito(clientito obj);
        
        void	runServer();
        void	removeClient(int id);
        void	removeFromChannel(int fd);

        //channel section
        void	createChannel(std::string command, int client_fd);
		void	parseJoinCommand(const std::string& command);
		void	proccessChannels(int clientfd);

        //Server main functions
        void    eventOnServerSock();
        void    eventOnClientSock();

        void    sendmessage(clientito &client, std::string _destination, std::string _message);
        void    broadcastMessage(Channel _channel, std::string _message, int _clientfd);
};
void	sendMsgToClient(int clientfd, std::string msg);
bool	check_port(std::string port);
void	programRequirement(int ac, char *port);
void	SocketAddrInfo(struct sockaddr_in& addr, int port_n);
void	interFace(Servrr obj);