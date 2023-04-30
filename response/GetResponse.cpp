#include "GetResponse.hpp"
#include <fstream>
#include <string>
std::string get_content_type(const char *path)
{
	const char *last_dot = strrchr(path, '.');
	if (last_dot)
	{
		if (strcmp(last_dot, ".css") == 0)
			return "text/css";
		if (strcmp(last_dot, ".csv") == 0)
			return "text/csv";
		if (strcmp(last_dot, ".gif") == 0)
			return "image/gif";
		if (strcmp(last_dot, ".htm") == 0)
			return "text/html";
		if (strcmp(last_dot, ".html") == 0)
			return "text/html";
		if (strcmp(last_dot, ".ico") == 0)
			return "image/x-icon";
		if (strcmp(last_dot, ".jpeg") == 0)
			return "image/jpeg";
		if (strcmp(last_dot, ".jpg") == 0)
			return "image/jpeg";
		if (strcmp(last_dot, ".js") == 0)
			return "application/javascript";
		if (strcmp(last_dot, ".json") == 0)
			return "application/json";
		if (strcmp(last_dot, ".png") == 0)
			return "image/png";
		if (strcmp(last_dot, ".pdf") == 0)
			return "application/pdf";
		if (strcmp(last_dot, ".svg") == 0)
			return "image/svg+xml";
		if (strcmp(last_dot, ".txt") == 0)
			return "text/plain";
		if (strcmp(last_dot, ".mp4") == 0)
			return "video/mp4";
	}
	return "application/octet-stream";
}

void err_403(clients_info &client)
{
//////////////////////////////////////////////////////////////////////////////


	if (client.flag_header == 0)
    {
        client.file.open("error/403.html", std::ios::in | std::ios::binary | std::ios::ate);
        client.file.seekg(0 , std::ios::end);
        client.size = client.file.tellg();
//        std::cout << client->size << std::endl;
        client.file.seekg(0, std::ios::beg);

        if (!client.file.is_open())
        {
            err_403(client);
            return;
        }
        client.header = "HTTP/1.1 200  Forbidden\r\n"
                         "Connection: close\r\n"
                         "Content-Type: text/html\r\n"
                         "Content-Length: " + std::to_string(client.size) + "\r\n\r\n";

        send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
//        std::cout << client->header << std::endl;
        client.flag_header = 1;
    }

    client.file.read(client.response, 1024);
    send(client.socket_client_id, client.response, 1024, 0);
    bzero(client.response, 1024);
}

void err_404(clients_info &client)
{
//////////////////////////////////////////////////////////////////////////////


    if (client.flag_header == 0)
    {
        client.file.open("error/404.html", std::ios::in | std::ios::binary | std::ios::ate);
        client.file.seekg(0 , std::ios::end);
        client.size = client.file.tellg();
//        std::cout << client->size << std::endl;
        client.file.seekg(0, std::ios::beg);

        if (!client.file.is_open())
        {
            err_403(client);
            return;
        }
        client.header = "HTTP/1.1 200 Not Found\r\n"
                         "Connection: close\r\n"
                         "Content-Type: text/html\r\n"
                         "Content-Length: " + std::to_string(client.size) + "\r\n\r\n";

        send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
//        std::cout << client->header << std::endl;
        client.flag_header = 1;
    }

    client.file.read(client.response, 1024);
    send(client.socket_client_id, client.response, 1024, 0);
    bzero(client.response, 1024);
}

void ok_200(clients_info &client, std::string file, std::string path)
{
//////////////////////////////////////////////////////////////////////////////

    if (client.flag_header == 0)
    {
        //
        client.path_file = file;
        //
        client.file.open(file, std::ios::in | std::ios::binary | std::ios::ate);
        client.file.seekg(0 , std::ios::end);
        client.size = client.file.tellg();
        client.file.seekg(0, std::ios::beg);

        if (!client.file.is_open()) {
            std::cout << "----------403--------\n";
            err_403(client);
            return;
        }
        client.header = "HTTP/1.1 200 OK\r\n"
                         "Connection: close\r\n"
                         "Content-Type: " + get_content_type(path.c_str()) + "\r\n"
                         "Content-Length: " + std::to_string(client.size) + "\r\n\r\n";

        send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
        client.flag_header = 1;
    }

    client.file.read(client.response, 1024);
    send(client.socket_client_id, client.response, 1024, 0);
    bzero(client.response, 1024);

}

std::string newpath(std::string path)
{
	size_t pos = -1;
	while (path.find("/", pos + 1) != std::string::npos)
		pos = path.find("/", pos + 1);
	path = path.substr(0, pos);
	if (path.empty())
		return ("/");
	return (path);
}

void GetResponse(std::deque<server> Srv, std::deque<clients_info> &client, int it_client)
{
	int check = 0;
	int i = 0;
	// request///
	std::string path = client[it_client].path;
//    std::cout << "HERREE : " << path << std::endl;
//    std::cout << "--------- path   --------"<< path << std::endl;
	////////////////////////
    std::deque<server>::iterator itSrv = Srv.begin();
	itSrv->root = "Server";
	//////////////

	std::string newPath = path;
	std::deque<location>::iterator itLoc = itSrv->locations.begin();
	std::string file;
	while (itLoc != itSrv->locations.end())
	{
		if (itLoc->path_location == newPath)
		{
			while (!itLoc->allow_methods[i].empty())
			{
				if (itLoc->allow_methods[i] == "GET")
					check++;
				i++;
			}
			check++;
			if (check != 2)
			{
				std::cout << "method not allow\n";
				err_404(client[it_client]);
			}
			file = itLoc->root + path;
//			std::cout << file << std::endl;
			std::ifstream file1(file);
			if (file1.good())
			{
//				std::cout << "exist\n";
				ok_200(client[it_client], file, path);
			}
			else
			{
				std::cout << "file not exist\n";
				err_404(client[it_client]);
			}
		}
		itLoc++;
		if (itLoc == itSrv->locations.end() && check == 0 && newPath != "/")
		{
			itLoc = itSrv->locations.begin();
			newPath = newpath(newPath);
		}
	}
	if (check == 0)
	{
//		std::cout << "=========no location===============\n";
		file = itSrv->root + path;
		std::ifstream file1(file);
		if (file1.good())
		{
//			std::cout << "exist\n";
			ok_200(client[it_client], file, path);
		}
		else
		{
			std::cout << "file not exist\n";
			err_404(client[it_client]);
		}
	}
}