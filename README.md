------------>READ THIS<---------------

do git pull, and revise the code

------------>SOLVED<---------------
- The problem I'm trying to solve now is that the server stuck in a blocking operation after receiving the first message from client, preventing it from processing further messages.
---> In getData(client_sock_fd); function, I used recv() which is a blocking call to receive data from the client, it will wait until it receives data or the connection is closed before continuing.

1 ==> I have Setted server socket to non-blocking mode
    if (fcntl(_sock_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

2 ==> I also declared a vectore with "struct pollfd" TYPE
    struct pollfd poll_fd; //here I declared a struct "poll_fd" to help us working with poll();
    std::vector<struct pollfd> fds;//Here I declared vectore with  "struct pollfd" TYPE

3 ==> first I pushed server socket fd, to My Vector, Then I used poll() function to handle multiplexing for us.
int ret = poll(fds.data(), fds.size(), -1);
if (ret == -1)
{
  perror("poll");
  exit(1);
}
