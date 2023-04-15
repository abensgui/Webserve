#ifndef GETRESPONSE_HPP
# define GETRESPONSE_HPP

# include <iostream>
# include <string>
# include "../configuration_file/configuration.hpp"
#include "../Server/socketServer.hpp"



void GetResponse(std::deque<server>::iterator itSrv, std::deque<clients_info>::iterator &client);

#endif /* ***************************************************** GETRESPONSE_H */