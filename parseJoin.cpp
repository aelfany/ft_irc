#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> parseJoinCommand(const std::string& command)
{
    std::vector<std::string> result;
    std::istringstream iss(command);
    std::string token;

    while (std::getline(iss, token, ','))
    {
        std::istringstream iss_token(token);
        std::string subtoken;
        while (std::getline(iss_token, subtoken, ' '))
        {
            if (!subtoken.empty())
                result.push_back(subtoken);
        }
    }
    return result;
}

int main()
{
    std::vector<std::string> parsed = parseJoinCommand("JOIN channel1");

    for (size_t i = 0; i < parsed.size(); ++i)
        std::cout << parsed[i] << std::endl;
}