#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include "../configuration_file/configuration.hpp"

#define MAX_REQUEST_SIZE 1024

class  clients_info
{
    public:
        socklen_t addr_len;
        struct  sockaddr_storage address; // ??
        int     socket_client_id;
        char    request[MAX_REQUEST_SIZE + 1];
        char    response[MAX_REQUEST_SIZE + 1];
        int     reciv;
        int     read_byte;
        long    content_lenght;
        int     r;
        int            flag_header;
        std::fstream   file;
        std::streamsize       size;
        std::string     header;
        clients_info();
        clients_info(const clients_info &);
        clients_info& operator=(const clients_info& obj);
        ~clients_info();
};

class SocketServer {
    public :
        struct timeval time;
        SocketServer();
        SocketServer(SocketServer const &cpy);
        ~SocketServer();

            void    run_server(std::deque<server> &servers);
        std::deque<clients_info> clients;

        std::deque<clients_info>::iterator get_client(int socket_client);
        void    remove_client(int socket_client);
        fd_set  wait_clients(int socket_server);
        void    connection(std::deque<server> &srv);
        void    type_content(std::string path_content);
};

#endif