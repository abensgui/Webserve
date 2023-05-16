#include "ft_Response.hpp"
#include <fstream>
#include <string>
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

std::deque<server>::iterator select_server(std::deque<server> &Srv, clients_info &client)
{
	std::deque<server>::iterator it = Srv.begin();

	while (it != Srv.end())
	{
		if (it->server_name == client.host && it->port == client.port)
			return (it);
		it++;
	}
	it = Srv.begin();
	while (it != Srv.end())
	{
		if (it->host == client.host && it->port == client.port)
			return (it);
		it++;
	}

	return (it);
}

std::deque<location>::iterator location_match(std::deque<location> &loc, std::string path)
{
	std::deque<location>::iterator it;
	while (1)
	{
		it = loc.begin();
		while (it != loc.end())
		{
			if (it->path_location == path)
				return (it);
			it++;
		}
		if (path == "/")
			break;
		path = newpath(path);
	}
	return (it);
}

void ft_redi(clients_info &client)
{
	client.header = "HTTP/1.1 301 Moved Permanently\r\n"
					"Location: " +
					client.itLoc->redirection  + "\r\n\n\r";
	send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
}

bool methodAllow(std::string met, std::deque<std::string> metA)
{
	std::deque<std::string>::iterator it = metA.begin();
	while (it != metA.end())
	{
		if (met == *it)
			return (1);
		it++;
	}
	return (0);
}
bool is_fileOrDir(std::string path)
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) == 0)
	{
		if (S_ISDIR(buffer.st_mode))
		{
			std::cout << path << " is a directory" << std::endl;
			return (1);
		}
	}
	return (0);
}

void ft_Response(std::deque<server> &Srv, clients_info &client)
{
	std::deque<location>::iterator itLoc;
	std::deque<server>::iterator itSrv;

	itSrv = select_server(Srv, client);
	if (itSrv == Srv.end())
		itSrv = Srv.begin();
	
	client.itSrv = itSrv;
	itLoc = location_match(itSrv->locations, client.path);
	client.itLoc = itLoc;
	if (!client.exit_status.first.empty())
	{
		statut_code(client, client.exit_status.first, client.exit_status.second);
	}
	else if (client.body.size() > itSrv->mx_cl_bd_size)
	{
		// 413 Request Entity Too Large
		statut_code(client, "413", "413 Request Entity Too Large");
	}
	else if (itLoc == itSrv->locations.end())
	{
		statut_code(client, "404", "404 Not Found");
	}
	else
	{
		if (methodAllow(client.method, itLoc->allow_methods))
		{
			if (!itLoc->redirection.empty())
			{
				client.flagRed = true;
				ft_redi(client);
			}
			else if (client.method == "GET")
				ft_get(client);
			else if (client.method == "POST")
				ft_post(client);
			else if (client.method == "DELETE")
				ft_delete(client);
		}
		else
		{
			statut_code(client, "405", "Method Not Allowed");
		}
	}
}