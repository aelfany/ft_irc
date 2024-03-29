// #include "client.hpp"
#include "Server.hpp"
#include <map>

typedef std::map<std::string, clientito> map_users;

class Channel
{
    private:
        // int			_id;
        std::string	_name;
        map_users	_users;
        Channel();
        Channel(const Channel& obj);
    public:
        Channel(std::string	_name);
        ~Channel();

		//getters
		std::string&	getChannelName();
		clientito&		getUser(std::string _nickname);

		void pushtomap(std::string _nickname, clientito& obj);
};