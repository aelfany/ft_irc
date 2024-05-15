/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelfany <abelfany@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 23:31:20 by abelfany          #+#    #+#             */
/*   Updated: 2024/05/15 23:35:38 by abelfany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/Server.hpp"
// #include "../../include/client.hpp"

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
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <netinet/in.h>
#include <netdb.h>
#include <sstream>
#include <map>

// Function to send HTTP request and receive response
std::string http_request(const std::string& host, const std::string& path)
{
    // I Create a socket here
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)//in case it failed
        throw std::runtime_error("socket failed");

    // Resolve host IP address
    struct hostent* server = gethostbyname(host.c_str());
    if (server == NULL)
        throw std::runtime_error("gethostbyname failed");

    // Prepare server address structure (sock_addr)
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(80);

    // here I try to Connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("connect failed");// in case failed

    // here I prepare HTTP request Format
    std::ostringstream request_stream;
    request_stream << "GET " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << host << "\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << "\r\n";
    std::string request = request_stream.str();

    // Send HTTP request
    if (send(sock, request.c_str(), request.length(), 0) < 0)
        throw std::runtime_error("send failed");

    // Receive HTTP response
    std::string response;
    char buffer[4096];
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer), 0)) > 0)//here receive the response
        response.append(buffer, bytes_received);
    if (bytes_received < 0)
        throw std::runtime_error("recv failed");//in case failed

    // Close the socket
    close(sock);

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
int main(int ac, char **av) {
    std::string api_key = "da36e6b8af17694f0fd72e962a735c3d";
    std::string city;
    
    int servsock = 3;
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(80);

    // here I try to Connect to the server
    // if (connect(servsock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    //     throw std::runtime_error("connect failed");// in case failed
    // send(servsock, "PASS 123", 8, 0);
    // send(servsock, "NICK wbot01", 11, 0);
    // send(servsock, "USER wbot", 9, 0);
    
    std::cout << "Enter the city name: ";
    std::string ss;
    std::getline(std::cin, city);

    std::string weather_data = get_weather(city, api_key);
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
        std::cout << it->first << it->second << std::endl;
    }
    
    return 0;
}