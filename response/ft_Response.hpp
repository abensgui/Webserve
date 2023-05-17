#ifndef FT_RESPONSE_HPP
#define FT_RESPONSE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <cstdio>
#include <sys/stat.h>
#include "../configuration_file/configuration.hpp"
#include "../Server/socketServer.hpp"
#include <ctime>
#include <sys/types.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

void ft_Response(std::deque<server> &Srv, clients_info &client);
void ft_send(clients_info &client);

std::string get_content_type(std::string path);
std::string get_extension(std::string path);
std::deque<std::string> get_env(clients_info &client);


void default_err(clients_info &client, std::string status);

void statut_code(clients_info &client, std::string err, std::string statut);
void ok_200(clients_info &client, std::string file);
void ft_get(clients_info &client);
void ft_post(clients_info &client);
void ft_delete(clients_info &client);


void listDir(clients_info &client, std::string file);
bool is_fileOrDir(std::string path);
bool methodAllow(std::string met, std::deque<std::string> metA);
void ft_redi(clients_info &client);
void listDir(clients_info &client, std::string file);

std::string delSp(std::string file);


void exec_cgi(clients_info &client,std::string file);


template <typename T>
std::string ft_to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


#endif /* ***************************************************** GETRESPONSE_H */