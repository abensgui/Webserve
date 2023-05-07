#include "../Server/socketServer.hpp"
#include "../response/ft_Response.hpp"

void SocketServer::parse_header(int client)
{
    std::string key, value, walo, line;
    int pos = 0;
    clients[client].header_req = clients[client].body.substr(0, clients[client].body.size() - (pos + 4));
    std::stringstream strm(clients[client].header_req);
    getline(strm, clients[client].method, ' ');
    getline(strm, clients[client].path, ' ');
    getline(strm, clients[client].protocol, '\n');
    getline(strm, walo, ' ');
    getline(strm, walo, '\n');
    pos = walo.find(":");
    if (pos == -1)
    {
        clients[client].host = walo;
    }
    else
    {
        clients[client].host = walo.substr(0, pos);
        clients[client].port = walo.substr(pos + 1, walo.length() - (pos + 2));
    }
    while (getline(strm, line, '\n'))
    {
        line = line.substr(0, line.size() - 1);
        pos = line.find(":");
        if (pos != -1)
        {
            key = line.substr(0, pos);
            value = line.substr(pos + 2);
            clients[client].map_request[key] = value;
        }
        else
            break;
        if (!key.compare("Content-Length"))
            clients[client].content_len = atol(value.c_str());
    }

    if (clients[client].method == "POST")
    {
        clients[client].is_post = 1;
        if (clients[client].map_request.find("Transfer-Encoding") == clients[client].map_request.end())
        {
            //501 Not implemented
        }
        if (clients[client].map_request.find("Content-Length") == clients[client].map_request.end() &&
        clients[client].map_request.find("Transfer-Encoding") == clients[client].map_request.end())
        {
            // 400 Bad request
        }
        if (clients[client].path.size() > 2048)
        {
            // 414 Request -URI Too Long
        }
    }
}

void SocketServer::parse_request(int it_client)
{
    int len_recived = recv(clients[it_client].socket_client_id, clients[it_client].request, MAX_SIZE, 0);
    if (len_recived < 0)
    {
        clients[it_client].fs.close();
        remove_client(clients[it_client].socket_client_id);
        clients[it_client].removed = 1;
    }
    else
    {
        if (clients[it_client].end_header_req == 0)
        {
            clients[it_client].body.append(clients[it_client].request, len_recived);
            int pol = clients[it_client].body.find("\r\n\r\n");
            if (pol != -1)
            {
                parse_header(it_client);
                if (clients[it_client].is_post == 1)
                    clients[it_client].body = clients[it_client].body.substr(pol + 4);
                clients[it_client].flag_res = 1;
                clients[it_client].end_header_req = 1;
            }
        }
        else if (clients[it_client].is_post == 1)
        {
            clients[it_client].body.append(clients[it_client].request, len_recived);
            if (clients[it_client].body.size() >= clients[it_client].content_len)
            {
                clients[it_client].post_finished = 1;
                clients[it_client].fs << clients[it_client].body;
            }
        }
    }
    if (len_recived < MAX_SIZE)
    {
        clients[it_client].flag_res = 1;
    }
}
