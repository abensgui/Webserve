#ifndef GETRESPONSE_HPP
# define GETRESPONSE_HPP

# include <iostream>
# include <string>
# include "../configuration_file/configuration.hpp"
#include "../Server/socketServer.hpp"



void GetResponse(std::deque<server> itSrv, std::deque<clients_info> &client, int it_client);

#endif /* ***************************************************** GETRESPONSE_H */