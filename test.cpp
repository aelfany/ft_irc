#include <iostream>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>

int OpenConnection(const char *hostname, int port)
{
    int sd;
    struct hostent *host;
    struct sockaddr_in addr = {0};
    if ((host = gethostbyname(hostname)) == NULL)
    {
        perror(hostname);
        abort();
    }

    sd = socket(PF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long *)(host->h_addr_list[0]);
    if (connect(sd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        perror(hostname);
        abort();
    }
    return sd;
}
int main()