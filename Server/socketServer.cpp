#include "socketServer.hpp"
#include "../response/GetResponse.hpp"

clients_info::clients_info() : file() {}

clients_info::clients_info(const clients_info &obj)
{
    (void)obj;
}

clients_info &clients_info::operator=(const clients_info &obj)
{
    file.copyfmt(obj.file);
    file.clear(obj.file.rdstate());
    file.seekg(0, std::ios::beg);
    file.seekp(0, std::ios::beg);
    file << obj.file.rdbuf();
    return *this;
}

clients_info::~clients_info() {}

SocketServer::SocketServer() : clients()
{
}

SocketServer::SocketServer(SocketServer const &cpy) : clients(cpy.clients)
{
}

SocketServer::~SocketServer()
{
}

std::deque<clients_info>::iterator SocketServer::get_client(int socket_client)
{
    (void)socket_client;
    std::deque<clients_info>::iterator it_client = clients.begin();
    //    while (it_client != clients.end())
    //    {
    //        if (it_client->socket_client_id == socket_client)
    //        {
    //            return (it_client);
    //        }
    //        it_client++;
    //    }
    clients_info add_new;
    add_new.addr_len = sizeof(it_client->address);
    clients.push_front(add_new);
    it_client = clients.begin();
    it_client->flag_header = 0;
    it_client->content_len = 0;
    it_client->flag_res = 0;
    it_client->flag_req = 0;
    it_client->len_rd = 0;
    return (it_client);
}

void SocketServer::remove_client(int socket_client)
{
    std::deque<clients_info>::iterator it_client = clients.begin();
    while (it_client != clients.end())
    {
        if (socket_client == it_client->socket_client_id)
        {
//            std::cout << "Client Num : " << &it_client << " is Diconnected\n";
//            std::cout<<"socket 1 : "<<it_client->socket_client_id<<std::endl;
//            it_client->socket_client_id = -1;
            close(it_client->socket_client_id);
            it_client = clients.erase(it_client);
//            std::cout<<"socket 2 : "<<&it_client<<std::endl;
            return;
        }
        it_client++;
    }
    std::cout << "Not found\n";
    return;
}

fd_set SocketServer::wait_clients(int socket_server)
{
    time.tv_usec = 10;
    time.tv_sec = 0;
    FD_ZERO(&reads);
    FD_ZERO(&writer);
    FD_SET(socket_server, &reads);
    FD_SET(socket_server, &writer);
    int max_socket = socket_server;
    std::deque<clients_info>::iterator it_client = clients.begin();
    while (it_client != clients.end())
    {
        if (it_client->socket_client_id > 0)
        {
            FD_SET(it_client->socket_client_id, &reads);
            if (it_client->socket_client_id > max_socket) {
                max_socket = it_client->socket_client_id;
            }
        }
        it_client++;
    }
    it_client = clients.begin();
    while (it_client != clients.end())
    {
        if (it_client->socket_client_id > 0)
        {
            FD_SET(it_client->socket_client_id, &writer);
            if (it_client->socket_client_id > max_socket) {
                max_socket = it_client->socket_client_id;
            }
        }
        it_client++;
    }
    if (select(max_socket + 1, &reads, &writer, 0, &time) < 0)
    {
        std::cout << "Select : Failed\n";
//        exit(1);
    }
    return (reads);
}

void SocketServer::parse_request(std::deque<clients_info>::iterator &it_client)
{
//    std::cout << "REQ : " << it_client->request << std::endl;
//    std::cout << "LEN : " << it_client->len_rd << std::endl;
//    std::cout << "CON : " << it_client->content_len << std::endl;
//    std::cout << "FLG HDR : " << it_client->flag_header << std::endl;
//    std::cout << "FLG RES : " << it_client->flag_res << std::endl;
    if (it_client->len_rd <= it_client->content_len) {
        it_client->len_rd += recv(it_client->socket_client_id, it_client->request, 1024, 0);
//        std::cout << "*****************************\n";
//        std::cout << "REQ : " << it_client->socket_client_id << std::endl;
    }
    else {
//        std::cout << "HANAAAAAAAA**********************\n";
        it_client->flag_res = 1;
    }
//    std::cout << "FLG REQ : " << it_client->flag_req << std::endl;
    if (it_client->flag_req == 0)
     {
         int head = 0;
         int pos;
         std::string key, value, walo;
         std::string req = it_client->request;
         std::stringstream strm(req);
         getline(strm, it_client->method, ' ');
         getline(strm, it_client->path, ' ');
         getline(strm, it_client->protocol, '\n');
         getline(strm, walo, ' ');
         getline(strm, walo, '\n');
         pos = walo.find(":");
         if (pos == -1) {
             it_client->host = walo;
         }
         else
         {
             it_client->host = walo.substr(0, pos);
             it_client->port = walo.substr(pos+1, walo.length() - (pos + 2)) ;
         }
         while (getline(strm, key, ':') && getline(strm, value, '\n'))
         {
             head += key.size() + value.size();
             if (!value.compare("\r\n\r\n")) {
//                 std::cout << "lenght1 : " << head << std::endl;
                 it_client->content_len = head;
             }
             it_client->map_request[key] = value;
         }
         it_client->content_len = atol(it_client->map_request["Content-Length"].c_str()) - it_client->content_len;
         it_client->flag_req = 1;
     }
//     std::cout << "cont_len : " << it_client->map_request["Content-Length"] << std::endl;
//     std::cout << "lenght2 : " << it_client->content_len << std::endl;
//    std::cout << "LEN1 : " << it_client->len_rd << std::endl;
//    bzero(it_client->request, 1024);
//    if (it_client->port.compare("8080"))
//std::string p = it_client->port;
//    std::cout << " WALO : |" << p << "|" << std::endl;
}

void SocketServer::connection(std::deque<server> &srv)
{
    std::deque<server>::iterator it_srv, it_response;
    std::deque<clients_info>::iterator it;
//    fd_set rd;
    while (1)
    {
        it = clients.begin();
        it_srv = srv.begin();
        while (it_srv != srv.end())
        {
            wait_clients(it_srv->socket_id);
            if (FD_ISSET(it_srv->socket_id, &reads))
            {
                it = get_client(-1);
//                std::cout << "FLG REQ : " << it->flag_req << std::endl;
                it->socket_client_id = accept(it_srv->socket_id, (struct sockaddr *)&(it->address), &it->addr_len);
                if (it->socket_client_id < 0)
                {
                    std::cerr << "Error in Connection\n";
                    return;
                }
//                parse_request(it);
//                std::cout << "PATH : " << it->path << std::endl;
                std::cout << "Client Num : " << it->socket_client_id << " is Connected with Server : [" << it_srv->host << ":" << it_srv->port << "]\n";
                it_response = it_srv;
                break;
            }
            it_srv++;
        }
        it = clients.begin();
//        std::cout<<clients.size()<<std::endl;
        while (it != clients.end())
        {
//            std::cout << "NUM  : " << it->socket_client_id << std::endl;
//            std::cout << "*****************************\n";
            if (it->socket_client_id > 0)
            {
//                std::cout << "NUM sOCK : " << it->socket_client_id << std::endl;
                if (FD_ISSET(it->socket_client_id, &writer)) {
                    parse_request(it);
//                    std::cout << it->port << " | " << std::endl;
                    if (it->flag_res == 1) {
                        GetResponse(it_response, it);
                    }
                    if (it->file.eof() && it->flag_res == 1) {
//                        std::cout << "*****************************\n";
                        it->file.close();
                        remove_client(it->socket_client_id);
//                        it = clients.begin();
//                        std::cout << "NUM sOCK : " << it->socket_client_id << std::endl;
//                        std::cout << "size() " << clients.size()<< std::endl;
                        break;
                    }
                }
            }
            it++;
        }
    }
}

void SocketServer::run_server(std::deque<server> &servers)
{
    std::deque<server>::iterator it = servers.begin();
    // 1 - creat socket for each server
    while (it != servers.end())
    {
        it->socket_id = socket(AF_INET, SOCK_STREAM, 0);
        if (it->socket_id < 0)
            exit(1);
        std::cout << "Creating socket... " << it->socket_id << std::endl;
        int opt_val = 1;
        setsockopt(it->socket_id, SOL_SOCKET, SO_REUSEPORT, &opt_val, sizeof(opt_val));
        // 2 - binding socket ma3a l port d server
        struct sockaddr_in ServerAddr;
        memset(&ServerAddr, 0, sizeof(ServerAddr));
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        ServerAddr.sin_port = htons(atoi(it->port.c_str()));
        if (bind(it->socket_id, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)))
        {
            std::cout << "Error In Bind Server\n";
            exit(1);
        }
        std::cout << "Binding socket to local address...\n";
        // 3- Listen to the Clients connection request
        if (listen(it->socket_id, 128))
        {
            std::cout << "No listen\n";
            exit(1);
        }
        std::cout << "Listening...\n";
        //            type_content("../Server/index.html");
        it++;
    }
    connection(servers);
}
