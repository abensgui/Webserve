#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <cstdio>
#include "../configuration_file/configuration.hpp"

#define MAX_SIZE 10000

class  clients_info
{
    public:
        socklen_t       addr_len;
        struct          sockaddr_storage address; // ??
        int             socket_client_id;
        char            request[MAX_SIZE];
        char            response[MAX_SIZE];
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
        // hada mo7al wax ba9in 5damin bih
        std::string filename_post;
        //end
        int         is_post;
        size_t      content_len;
        int         end_header;
        int         flag_res;
        bool        flagRed;
        int         flag_header;
        std::fstream fs;
        //parse Req
        std::string header_req;
        std::string body;
        int         end_header_req;
        bool        clear_client;
        int         post_finished;
        int         removed;
        int         is_chunk;
        int         len_chunk;

};

class SocketServer {
    public :
        fd_set reads, writer;
        struct timeval time;
        struct addrinfo ServerAddr;
        SocketServer();
        SocketServer(SocketServer const &cpy);
        ~SocketServer();

        void    run_server(std::deque<server> &servers);
        std::deque<clients_info> clients;

        int     get_client();
        int     remove_client(int socket_client);
        void    wait_clients(std::deque<server> &srv);
        void    connection(std::deque<server> &srv);
        //parse request
        void    parse_request(int);
        void    parse_header(int);
};

#endif
