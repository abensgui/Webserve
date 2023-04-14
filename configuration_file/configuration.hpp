#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <map>
#include <string>

struct location
{
    std::string path_location, root, upload_pass, auto_index, redirection;
    std::deque<std::string> allow_methods;
    std::deque<std::string> index;
};

struct server
{
    int socket_id;
    std::string host, port, server_name;
    unsigned int mx_cl_bd_size;
    std::map<std::string, std::string> err_pages;
    std::deque<location> locations;
};

class confi
{
    public:
        std::deque<server> servers;
        void    split_string(std::string &, location &lc, char c);
        int     setup_configuration(std::string);
};

#endif