#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <cstdio>
#include "../configuration_file/configuration.hpp"

#define MAX_REQUEST_SIZE 1024

class  clients_info
{
    public:
        socklen_t       addr_len;
        struct          sockaddr_storage address; // ??
        int             socket_client_id;
        char            request[MAX_REQUEST_SIZE];
        char            response[MAX_REQUEST_SIZE];
        std::string     path_file;
        std::fstream    file;
        std::streamsize size;
        std::string     header;
        std::string     method, path, protocol, host, port;
        std::map<std::string, std::string > map_request;
        clients_info();
        clients_info(const clients_info &);
        clients_info& operator=(const clients_info& obj);
        ~clients_info();
        std::string filename_post;
        size_t      content_len;
        int         content_len_exist;
        int         chunked_exist;
        int         end_header;
        size_t      end_chunk;
        size_t      len_rd;
        int         flag_res;
        int         flag_req;
        bool        flagRed;
        int         flag_header;
        std::fstream fs;

};
//attachment; filename="bear.jpg"
class SocketServer {
    public :
        fd_set reads, writer;
        struct timeval time;
        SocketServer();
        SocketServer(SocketServer const &cpy);
        ~SocketServer();

        void    run_server(std::deque<server> &servers);
        std::deque<clients_info> clients;

        int get_client();
//        void    remove_client(int socket_client);
        int remove_client(int socket_client);
        void  wait_clients(std::deque<server> &srv);
        void    connection(std::deque<server> &srv);
        //parse request
        void    parse_request(int);
};

#endif
