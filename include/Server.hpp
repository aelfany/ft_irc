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


typedef std::vector<clientito> clientVect;//vector of clients
typedef std::map<std::string, Channel> channelsMap;//map of channels
typedef std::vector<std::string> JoinVect;//vector for Parse join
typedef std::vector<struct pollfd> PollfdVect;


class Servrr
{
    private:
        int							_portNumber;
        std::map<std::string, std::string> bot;
        std::string					_password;
        std::string					host;
        PollfdVect					_fds;
        sockaddr_in					_addr;
        int							_sock_fd;
        clientVect	                _clients;
        channelsMap					_channels;
        JoinVect					_result;
        JoinVect                    args;
        std::vector<std::pair<std::string,std::string> > mod;
        Servrr();
    public:
        size_t	_index;
        void command(std::string buffer, size_t i);
        Servrr(int portNumber, std::string passw);
        ~Servrr();
        int parsNick(clientito& client);
        
        int modeSplit(std::string arg, size_t i);
        // void parsUser(clientito& client);
        void	            proccessChannels(int clientfd, std::string nick);
        void                createChannel(std::string command, int clientfd, std::string nick);
		void	            auth2(std::string str, clientito& client);
        void                trimUser(const std::string& str);
        Channel &           getChannel(std::string channel);
        void                trimSpaces(const std::string& str, bool x);
        void                kick(std::string buffer, size_t i);
        void                split(std::string buffer, size_t i);
		std::string&		getPassword();
		unsigned int		getPort();
		int					getSockFd();
		PollfdVect&			getPollfdVect();
		struct sockaddr_in&	getSockAddr();
        clientito&			getClientitoByIndex(int index);
        clientito&			getClientitoByfd(int clientfd);
        void                Topic(std::string nick, size_t i);
        std::string         _time(std::string nick);

		void	setSockAddr();
		void	setSockFd(int sock_fd);
		void	setClientito(clientito obj);
        
        void	runServer();
        void	removeClient(int id);
        void	removeFromChannel(int fd);

        //channel section
        int checkNick(clientito& client);
        clientito&	getClient(std::string nick);
		void	parseJoinCommand(const std::string& command);
        bool    alreadyAmember(int clientfd, Channel channel);
        std::string tolowercases(std::string _str1);

        //Server main functions
        void    eventOnServerSock();
        void    eventOnClientSock();

        void eraseChannel(std::string _name);

        void    sendmessage(clientito &client, std::string _destination, std::string _message);
        void    broadcastMessage(Channel _channel, std::string _message, int _clientfd);
        void    SendToAll(Channel _channel, std::string _message);
};
void	sendMsgToClient(int clientfd, std::string msg);
int     check_port(std::string port, std::string pass);
void    programRequirement(int ac, char *port, char *pass);
void	SocketAddrInfo(struct sockaddr_in& addr, int port_n);
void	interFace(Servrr obj);