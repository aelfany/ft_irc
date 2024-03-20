#include "../../include/Server.hpp"

void eventOnServerSock(int servSockfd, struct sockaddr_in addr, std::vector<struct pollfd>& fds)
{
    socklen_t client_addr_l = sizeof(addr);
    int client_sock_fd = accept(servSockfd, (struct sockaddr *)&addr, &client_addr_l);
    if (client_sock_fd == -1)
        perror("accepttt");
    else
    {
        struct pollfd poll_fd;
        poll_fd.fd = client_sock_fd;
        poll_fd.events = POLLIN;
        fds.push_back(poll_fd);
        std::cout << "New connection from client with fd: " << client_sock_fd << std::endl;
    }
}

void eventOnClientSock(std::vector<pollfd>& fds, size_t& i)
{
    // Event on client socket (data available)
    int client_sock_fd = fds[i].fd;
    char buffer[1024];
    ssize_t recvData = recv(client_sock_fd, buffer, sizeof(buffer), 0);
    if (recvData == -1)
        perror("recv");
    else if (recvData == 0)
    {
        // If client disconnected print this==> && close its socket && erase its data in our vector
        std::cout << "Client "<< client_sock_fd << " disconnected" << std::endl;
        close(client_sock_fd);
        fds.erase(fds.begin() + i);
        i--;
    }
    else
        std::cout << "Received: " << std::string(buffer, recvData) << std::endl;
}

int main(int ac, char **av)
{
    struct pollfd poll_fd;
    struct sockaddr_in addr;
    std::vector<struct pollfd> fds;//declare vectore with type struct pollfd

    simpleRules(ac, av[1]);
    int portN = atoi(av[1]);

    //class Servvr takes a port number & a password
    Servrr server1(portN, av[2]);
    SocketAddrInfo(addr, portN);
    server1.runServer(addr);
    interFace(server1);

    poll_fd.fd = server1.getSockFd();//sock server fd
    poll_fd.events = POLLIN;
    fds.push_back(poll_fd);
    while(true)
    {
        //Hna 3tyet l poll() struct(lli fiha: int fd, short event), o 3teytha size dyal vector, o time lli ghadi tsena (1000ms == 1second).
        //db poll() ghadi t-checki server_socket_fd, kulla 1 second, wash jato shi event wla la. OK?
        //ila jat shi event, poll() ghadi t-settey REVENTS b 1 ==> "poll_fd.revents = 1"
        //mn ba3d, ghadi n't-checki wash REVENTS & POLLIN == TRUE ? 
        //mn ba3d, ghadi n't-checki hadak REVENTS wash dyal ServerSocket wla ClientSocket, OK?
        //So, ila kan dyal SERVER, rah new client bgha y't-connecta m3a server. O ila kan dyal CLIENT, rah new DATA has been sent to SERVER.
        //ila kan new Client bgha y't-connecta, ghadi t-Acceptih o t7etto fl VECTOR (std::vector<struct pollfd> fds).
        //ila bgha y-sered DATA l server, receiver DATA, using recv(). ila returnat value > 0 printey DATA, ila returnat 0 rah deconnecta(y3ni close socket && remove it from vector).
        int ret = poll(fds.data(), fds.size(), 1000);
        if (ret == -1)
        {
            perror("poll");
            exit(1);
        }
        // Check for events on each client socket
        for (size_t i = 0; i < fds.size(); i++)
        {
            //poll() ghadi t-checki ila kan kain shi event f "server socket wla client socket", but f awal merra ghadi ykun ghi server socket (vector fih ghi struct dyal server, ba9i makinsh clients)
            //Poll() ghatb9a tsena ta ywslha shi event(POLLIN || POLLOUT), fsh ywslha event ghadi t-settey fds[i].revent = 1
            //Next: ghadi n't-checki: fds[i].revent & POLLIN ==> (revent = 1 & POLLIN = 1 == TRUE). ila kanu bjuj positive y3ni condition TRUE
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == server1.getSockFd()) // Event on server socket (new connection)
                    eventOnServerSock(server1.getSockFd(), addr, fds);
                else // Event on client socket (data available)
                    eventOnClientSock(fds, i);
            }
        }
    }
}
