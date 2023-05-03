#ifndef FT_RESPONSE_HPP
#define FT_RESPONSE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include "../configuration_file/configuration.hpp"
#include "../Server/socketServer.hpp"

void ft_Response(std::deque<server> &Srv, clients_info &client);

#endif /* ***************************************************** GETRESPONSE_H */