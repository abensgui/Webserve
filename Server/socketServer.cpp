#include "socketServer.hpp"
#include "../response/ft_Response.hpp"

clients_info::clients_info() : file() {}

clients_info::clients_info(const clients_info &obj)
{
    *this = obj;
}

clients_info &clients_info::operator=(const clients_info &obj)
{
    addr_len = obj.addr_len;
    address = obj.address;
    socket_client_id = obj.socket_client_id;
    path_file = obj.path_file;
    file.open(obj.path_file, std::ios::in | std::ios::binary | std::ios::ate);
    file << obj.file.rdbuf();
    size = obj.size;
    method = obj.method;
    path = obj.path;
    protocol = obj.protocol;
    host = obj.host;
    port = obj.port;
    map_request = obj.map_request;
    content_len = obj.content_len;
    end_header = obj.end_header;
    flag_res = obj.flag_res;
    clear_client = obj.clear_client;
    post_finished = obj.post_finished;
    removed = obj.removed;
    is_chunk = obj.is_chunk;
    exit_status = obj.exit_status;
    header = obj.header;
    flag_header = obj.flag_header;
    flagRed = obj.flagRed;
    fs.open(path, std::fstream::out | std::fstream::app);
    fs << obj.file.rdbuf();
    post_file = obj.post_file;
    itLoc = obj.itLoc;
    itSrv = obj.itSrv;
    pid = obj.pid;
    file_aa = obj.file_aa;
    flag_ff = obj.flag_ff;
    send_hed = obj.send_hed;
    exe_cgi = obj.exe_cgi;

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

int SocketServer::get_client()
{
    int index_client = 0;
    clients_info add_new;
    clients.push_back(add_new);
    index_client = clients.size() - 1;
    clients[index_client].addr_len = sizeof(clients[index_client].address);
    bzero(clients[index_client].request, MAX_SIZE);
    bzero(clients[index_client].response, MAX_SIZE);
    clients[index_client].content_len = 0;
    clients[index_client].flag_header = 0;
    clients[index_client].flag_res = 0;
    clients[index_client].flagRed = 0;
    clients[index_client].end_header = 0;
    clients[index_client].is_post = 0;
    clients[index_client].end_header_req = 0;
    clients[index_client].clear_client = 0;
    clients[index_client].post_finished = 0;
    clients[index_client].removed = 0;
    clients[index_client].is_chunk = 0;
    clients[index_client].flag_ff = 0;
    clients[index_client].send_hed = 0;
    clients[index_client].exe_cgi = 0;
    return (index_client);
}

int SocketServer::remove_client(int socket_client)
{
    size_t it_client = 0;
    while (it_client < clients.size())
    {
        if (socket_client == clients[it_client].socket_client_id)
        {
            close(clients[it_client].socket_client_id);
            clients.erase(clients.begin() + (it_client));
            return it_client;
        }
        it_client++;
    }
    return 0;
}

void SocketServer::wait_clients(std::deque<server> &srv)
{
    FD_ZERO(&reads);
    FD_ZERO(&writer);
    size_t it_srv = 0;
    int max_socket = 0;
    while (it_srv < srv.size())
    {
        FD_SET(srv[it_srv].socket_id, &reads);
        if (srv[it_srv].socket_id > max_socket)
            max_socket = srv[it_srv].socket_id;
        it_srv++;
    }
    size_t it_client = 0;
    while (it_client < clients.size())
    {
        FD_SET(clients[it_client].socket_client_id, &reads);
        FD_SET(clients[it_client].socket_client_id, &writer);
        if (clients[it_client].socket_client_id > max_socket)
            max_socket = clients[it_client].socket_client_id;
        it_client++;
    }
    if (select(max_socket + 1, &reads, &writer, 0, 0) < 0)
    {
        std::cout << "Select : Failed\n";
        exit(1);
    }
}

void SocketServer::connection(std::deque<server> &srv)
{
    size_t it_srv;
    size_t it_client;
    while (1)
    {
        it_srv = 0;
        it_client = 0;
        wait_clients(srv);
        while (it_srv < srv.size())
        {
            if (FD_ISSET(srv[it_srv].socket_id, &reads))
            {
                it_client = get_client();
                clients[it_client].socket_client_id = accept(srv[it_srv].socket_id, (struct sockaddr *)&(clients[it_client].address), &clients[it_client].addr_len);
                if (clients[it_client].socket_client_id < 0)
                {
                    std::cerr << "Error in Connection\n";
                    return;
                }
                fcntl(clients[it_client].socket_client_id, F_SETFL, O_NONBLOCK);
                break;
            }
            it_srv++;
        }
        it_client = 0;
        while (it_client < clients.size())
        {
            if (FD_ISSET(clients[it_client].socket_client_id, &reads))
            {
                if (clients[it_client].flag_res == 0)
                    parse_request(it_client);
            }
            if (FD_ISSET(clients[it_client].socket_client_id, &writer))
            {
                if (clients[it_client].flag_res == 1)
                {
                    if (clients[it_client].flag_header == 0)
                        ft_Response(srv, clients[it_client]);
                    ft_send(clients[it_client]);
                }
                if (clients[it_client].clear_client == true && clients[it_client].removed == 0)
                {
                    clients[it_client].file.close();
                    remove_client(clients[it_client].socket_client_id);
                }
            }
            it_client++;
        }
    }
}

void SocketServer::run_server(std::deque<server> &servers)
{
    size_t i = 0;
    // 1 - creat socket for each server
    while (i < servers.size())
    {
        memset(&ServerAddr, 0, sizeof(ServerAddr));
        ServerAddr.ai_family = AF_INET;
        ServerAddr.ai_socktype = SOCK_STREAM;
        struct addrinfo *Res;
        getaddrinfo(servers[i].host.c_str(), servers[i].port.c_str(), &ServerAddr, &Res);
        servers[i].socket_id = socket(Res->ai_family, Res->ai_socktype, Res->ai_protocol);
        if (servers[i].socket_id < 0)
            exit(1);
        fcntl(servers[i].socket_id, F_SETFL, O_NONBLOCK);
        std::cout << "Creating socket... " << servers[i].socket_id << std::endl;
        // // 2 - binding socket ma3a l port d server
        if (bind(servers[i].socket_id, Res->ai_addr, Res->ai_addrlen))
        {
            std::cout << "Error In Bind Server\n";
            exit(1);
        }
        std::cout << "Binding socket to local address...\n";
        freeaddrinfo(Res);
        // 3- Listen to the Clients connection request
        if (listen(servers[i].socket_id, 128))
        {
            std::cout << "No listen\n";
            exit(1);
        }
        std::cout << "Listening...\n";
        i++;
    }
    connection(servers);
}
