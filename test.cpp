#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <string>
#include <string>
#include <sstream>

std::string  _time(std::string nick) {
    time_t now = time(0);
    std::string tmp = ctime(&now);
    std::string time;
    std::stringstream s(tmp);
    std::string arr[5];
    for(int a = 0; s >> arr[a]; a++) {}
    time += nick + " " + arr[1] + " " + arr[2] + ", " + arr[4] + " at " + arr[3];
    std::cout << time << std::endl;
    return time;
}
int main() {
    _time("abdo");
}