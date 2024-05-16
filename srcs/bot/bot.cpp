#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <netinet/in.h>
#include <sstream>
#include <map>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <netdb.h>

int connectToServer(const char* hostname, int port)
{
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        throw std::runtime_error("Socket creation failed");

    // Resolve host IP address
    struct hostent* server = gethostbyname(hostname);
    if (server == NULL)
    {
        close(sockfd);
        throw std::runtime_error("Failed to resolve host");
    }

    // Prepare server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sockfd);
        throw std::runtime_error("Connection failed");
    }
    return sockfd;
}

// Function to send HTTP request and receive response
std::string http_request(const std::string& host, const std::string& path)
{
    // Create socket && connect to server
    int sockfd = connectToServer(host.c_str(), 80);

    // here I prepare HTTP request Format
    std::ostringstream request_stream;
    request_stream << "GET " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << host << "\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << "\r\n";
    std::string request = request_stream.str();

    // Send HTTP request
    if (send(sockfd, request.c_str(), request.length(), 0) < 0)
        throw std::runtime_error("send failed");

    // Receive HTTP response
    std::string response;
    char buffer[4096];
    int bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)//here receive the response
        response.append(buffer, bytes_received);
    if (bytes_received < 0)
        throw std::runtime_error("recv failed");//in case failed

    // Close the socket
    close(sockfd);

    return response;
}

// Function to fetch weather data from OpenWeatherMap API
std::string get_weather(const std::string& city, const std::string& api_key)
{
    std::string host = "api.openweathermap.org";
    std::string path = "/data/2.5/weather?q=" + city + "&appid=" + api_key;
    return http_request(host, path);
}

int findWordIndex(const std::string& buffer, const std::string& word) {
    size_t index = buffer.find(word);
    if (index != std::string::npos) {
        return static_cast<int>(index + word.length());
    }
    return -1;
}

std::string word(const std::string& buffer, size_t index) {
    std::string res;
    for(;buffer[index] != ',' && buffer[index]; index++) {
        if(buffer[index] != '"' && buffer[index] != '}')
            res.push_back(buffer[index]);
    }
    return res;
}


void sendMsgToClient(int clientfd, std::string msg)
{
    if (!msg.empty())
    {
        ssize_t bytes = send(clientfd, msg.c_str(), msg.length(), 0);
        if (bytes == -1)
            throw std::runtime_error("Failed to send message to server");
    }
    usleep(100);
}

void    parseData(const std::string& command, std::vector<std::string> &botCmd)
{
    std::stringstream iss(command);
	std::string part;

	while (iss >> part)
        botCmd.push_back(part);
    botCmd.push_back("");
}


void parceWeather(std::string weather_data, int sockfd)
{
    std::string str = weather_data;
    std::string search[6] = {"\"temp\":", "\"temp_min\":", "\"temp_max\":", "\"humidity\":", "\"speed\":", "\"country\":"};
    std::string dt[6] = {"Temperature: ", "Minimum Temperature: ", "Maximum Temperature: ", "Humidity: ", "Wind Speed: ", "Country: "};
    std::vector<std::pair<std::string, std::string> > data;
    std::string res;
    int index = 0;
    for(size_t a = 0; a < 6; a++) {
        index = findWordIndex(str, search[a]);
        res = word(str, index);
        data.push_back(std::make_pair(dt[a], res));
    }
    std::vector<std::pair<std::string, std::string> >::iterator it = data.begin(); 
    for(; it != data.end(); it++) {
        std::string msg = it->first + it->second+"\r\n";
        std::cout << it->first << it->second << std::endl;
        sendMsgToClient(sockfd, msg);
    }
}


void receivedata(int sockfd)
{
    std::string api_key = "da36e6b8af17694f0fd72e962a735c3d";
    char buffer[1024];

    // Receive data from the server
    int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes_received < 0)
    {
        close(sockfd);
        throw std::runtime_error("Failed to receive data from server");
    }
    else if (bytes_received == 0)
    {
        // Connection closed by server
        close(sockfd);
        throw std::runtime_error("Connection closed by server");
    }
    buffer[bytes_received] = '\0';
    std::cout << buffer << std::endl;

    std::vector<std::string> botCmd;
    parseData(buffer, botCmd);
    if(botCmd[1] == "PRIVMSG")
    {
        if(botCmd.size() < 4)
            return ;
        std::string weather_data = get_weather(botCmd[4], api_key);
        parceWeather(weather_data, sockfd);
    }

}

int main(int ac, char **av)
{
    int sockfd = connectToServer("localhost", 4545);
    bool authed = false;
    while (true)
    {
        if(authed == false)
        {
            try
            {
                sendMsgToClient(sockfd, "pass 123\r\n");
                sendMsgToClient(sockfd, "nick fuckinBoot\r\n");
                sendMsgToClient(sockfd, "user fuckinBoot\r\n");
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            authed = true;
            if(authed)
            {
                sendMsgToClient(sockfd, "Hello human, I'm a stupid bot and I'm going to provide you with useless info\r\n");
                sendMsgToClient(sockfd, "Try: BOT yourcity\r\n");
            }
        }
        receivedata(sockfd);
    }
    return 0;
}