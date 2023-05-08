#ifndef FT_RESPONSE_HPP
#define FT_RESPONSE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <cstdio>
#include <sys/stat.h>
#include "../configuration_file/configuration.hpp"
#include "../Server/socketServer.hpp"
#include <ctime>

void ft_Response(std::deque<server> &Srv, clients_info &client);

std::string get_content_type(const char *path);
std::string get_extension(const char *path);


void ft_202(clients_info &client, std::map<std::string, std::string> err_pages);
void err_403(clients_info &client, std::map<std::string, std::string> err_pages);
void err_500(clients_info &client, std::map<std::string, std::string> err_pages);
void err_405(clients_info &client, std::map<std::string, std::string> err_pages);
void err_404(clients_info &client, std::map<std::string, std::string> err_pages);
void ok_200(clients_info &client, std::string file, std::map<std::string, std::string> err_pages);
void created_201(clients_info &client, std::map<std::string, std::string> err_pages);

void ft_get(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages);
void ft_post(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages);
void ft_delete(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages);


void listDir(clients_info &client, std::string file, std::deque<location>::iterator itLoc);
bool is_fileOrDir(std::string path);
bool methodAllow(std::string met, std::deque<std::string> metA);
void ft_redi(std::string redi, clients_info &client);
void listDir(clients_info &client, std::string file);

std::string delSp(std::string file);


#endif /* ***************************************************** GETRESPONSE_H */