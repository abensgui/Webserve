#include "configuration_file/configuration.hpp"
#include "Server/socketServer.hpp"

//int get_cli(SocketServer &skt)
//{
//    int i = 0;
//    clients_info client1;
////        client1.socket_client_id = i;
//    skt.clients.push_back(client1);
//    i = skt.clients.size() - 1;
//    skt.clients[i].socket_client_id = i;
//    return (i);
//}
//
//std::vector<clients_info>::iterator rmv_cli(int index, SocketServer &skt)
//{
////    size_t it_client = 0;
//    std::vector<clients_info>::iterator it_client = skt.clients.begin();
//    while (it_client != skt.clients.end())
//    {
//        if (index == it_client->socket_client_id)
//        {
//            skt.clients.erase(it_client);
//            std::cout << "HEREEEE" << it_client->socket_client_id << "\n";
//            return (it_client);
//        }
//        it_client++;
//    }
//    return it_client;
//}

int main(int ac , char **av)
{
//    if (ac != 2)
//        exit (1);
//
////    (void)av;
//    confi cfg;
//    SocketServer skt;
//    cfg.setup_configuration(av[1]);
//
//    for (int i = 0; i < 10; i++)
//    {
//        clients_info name;
//        name.socket_client_id = i;
//        skt.clients.push_back(name);
//    }
////
//    std::cout << "===============BEFORE==============\n";
//    for (size_t i = 0; i < skt.clients.size(); i++)
//    {
//        std::cout << " " << skt.clients[i].socket_client_id;
//    }
//    std::cout << "\n==============AFTER=================\n";
//    std::vector<clients_info>::iterator  it = skt.clients.begin();
//
//    while (it != skt.clients.end())
//    {
//        if (it->socket_client_id == 2)
//        {
//            it = skt.clients.erase(it);
//            break;
//        }
//        it++;
//
//    }
//    for (size_t i = 0; i < skt.clients.size(); i++)
//    {
//        std::cout << " " << skt.clients.at(i).socket_client_id;
//    }
//    std::cout << "\n";
//}
    //========================================================================

    signal(SIGPIPE, SIG_IGN);
    if (ac != 2)
        exit (1);
    confi cfg;
    SocketServer skt;
    cfg.setup_configuration(av[1]);
    skt.run_server(cfg.servers);
//    std::deque<server>::iterator it = cfg.servers.begin();
//    while (it != cfg.servers.end())
//    {
//        int pid = fork();
//        if (pid == 0)
//        {
//            std::cout << "host : " << it->host << std::endl;
//            std::cout << "port : " << it->port << std::endl;
//            std::cout << "server_name : " << it->server_name << std::endl;
//            SocketServer skt(it->host, it->server_name, atoi(it->port.c_str()));
//            skt.Socket_server();
//        }
//        it++;
//    }
//    while(wait(NULL))
//        ;
//    std::deque<server>::iterator it = cfg.servers.begin();
//    while (it != cfg.servers.end())
//    {
//        std::map<std::string, std::string>::iterator it_map = it->err_pages.begin();
//
//        std::cout << "host : " << it->host << std::endl;
//        std::cout << "port : " << it->port << std::endl;
//        std::cout << "server_name : " << it->server_name << std::endl;
//        std::cout << "mx_cl_bd_size : " << it->mx_cl_bd_size << std::endl;
//        while (it_map != it->err_pages.end())
//        {
//            std::cout << "Err_pages : " << it_map->first << " | " << it_map->second << std::endl;
//            it_map++;
//        }
//        std::cout << "==============================================\n";
//        std::deque<location>::iterator itr = it->locations.begin();
//
//        while (itr != it->locations.end())
//        {
//            std::deque<std::string>::iterator it_met = itr->methods.begin();
//            std::deque<std::string>::iterator it_ind = itr->index.begin();
//
//            std::cout << "path_location : " << itr->path_location << std::endl;
//            std::cout << "root : " << itr->root << std::endl;
//            std::cout << "upload_pass : " << itr->upload_pass << std::endl;
//            std::cout << "auto_index : " << itr->auto_index << std::endl;
//            std::cout << "redirection : " << itr->redirection << std::endl;
//            std::cout << "methods : ";
//            while (it_met != itr->methods.end())
//            {
//                std::cout << *it_met << " ";
//                it_met++;
//            }
//            std::cout << "\nindex : ";
//            while (it_ind != itr->index.end())
//            {
//                std::cout << *it_ind << " ";
//                it_ind++;
//            }
//            std::cout << "\n*******************************************\n";
//            itr++;
//        }
//        std::cout << "==============================================\n\n\n";
//        it++;
//    }
}
