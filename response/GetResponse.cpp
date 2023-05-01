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
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		//        std::cout << client.size << std::endl;
		client.file.seekg(0, std::ios::beg);

		if (!client.file.is_open())
		{
			err_403(client);
			return;
		}
		client.header = "HTTP/1.1 200  Forbidden\r\n"
						"Connection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		//        std::cout << client.header << std::endl;
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
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		//        std::cout << client.size << std::endl;
		client.file.seekg(0, std::ios::beg);

		if (!client.file.is_open())
		{
			err_403(client);
			return;
		}
		client.header = "HTTP/1.1 200 Not Found\r\n"
						"Connection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		//        std::cout << client.header << std::endl;
		client.flag_header = 1;
	}

	client.file.read(client.response, 1024);
	send(client.socket_client_id, client.response, 1024, 0);
	bzero(client.response, 1024);
}

void ok_200(clients_info &client, std::string file)
{
	//////////////////////////////////////////////////////////////////////////////

	if (client.flag_header == 0)
	{

		client.file.open(file, std::ios::in | std::ios::binary | std::ios::ate);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);

		if (!client.file.is_open())
		{
			std::cout << "----------403--------\n";
			err_403(client);
			return;
		}
		client.header = "HTTP/1.1 200 OK\r\n"
						"Connection: close\r\n"
						"Content-Type: " +
						get_content_type(file.c_str()) + "\r\n"
														 "Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}

	client.file.read(client.response, 1024);
	send(client.socket_client_id, client.response, 1024, 0);
	bzero(client.response, 1024);
}

void listDir(clients_info &client)
{
	std::stringstream output("index.html");
	output << "<html><body><ul>";

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(client.path.substr(1, client.path.length() - 1).c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			output << "<li>" << ent->d_name << "</li>";
		}
		closedir(dir);
	}
	else
	{
		perror("Unable to open directory");
	}
	output << "</ul></body></html>";
	std::string s;
	output >> s;
	client.header = "HTTP/1.1 200 OK\r\n"
					"Connection: close\r\n"
					"Content-Type: "
					"text/html\r\n"
					"Content-Length: " +
					std::to_string(s.size()) +
					"\r\n\r\n";
	send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
	send(client.socket_client_id, output.str().c_str(), 108, 0);
	client.flagRed = true;
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

std::deque<server>::iterator select_server(std::deque<server> &Srv, clients_info &client)
{
	std::deque<server>::iterator it = Srv.begin();

	while (it != Srv.end())
	{
		if ((it->host == client.host || it->server_name == client.host) && it->port == client.port)
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

void ft_redi(std::string redi, clients_info &client)
{
	std::cout << "here" << std::endl;
	(void)redi;
	client.header = "HTTP/1.1 301 Moved Permanently\r\n"
					"Location: http://www.example.org\r\n";
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
	std::cout << "-------------seg\n";
	return (0);
}
int is_fileOrDir(std::string path)
{
	std::cout << "--------------on----------  " << std::endl;
	path = path.substr(1, path.length() - 1);
	struct stat buffer;
	if (stat(path.c_str(), &buffer) == 0)
	{
		if (S_ISDIR(buffer.st_mode))
		{
			std::cout << path << " is a directory" << std::endl;
			return (1);
		}
		else if (S_ISREG(buffer.st_mode))
		{
			std::cout << path << " is a file" << std::endl;
			return (2);
		}
		else
		{
			std::cout << path << " is not a file or directory" << std::endl;
			return (3);
		}
	}
	else
	{
		std::cout << path << " does not exist" << std::endl;
		return (0);
	}
	return (0);
}

void GetResponse(std::deque<server> &Srv, clients_info &client)
{
	std::deque<location>::iterator itLoc;
	std::deque<server>::iterator itSrv;
	size_t i = 0;
	std::string file;

	itSrv = select_server(Srv, client);
	if (itSrv == Srv.end())
		itSrv = Srv.begin();

	itLoc = location_match(itSrv->locations, client.path);

	std::cout << "Loc _____ " << client.path << std::endl;

	if (itLoc == itSrv->locations.end())
	{
		std::cout << "no location\n";
	}

	if (methodAllow(client.method, itLoc->allow_methods))
	{
		if (!itLoc->redirection.empty())
		{
			client.flagRed = true;
			ft_redi(itLoc->redirection, client);
		}
		else
		{
			if (is_fileOrDir(client.path) == 1)
			{

				if (itLoc->auto_index == "on")
					listDir(client);
				else
				{
					while (i < itLoc->index.size())
					{
						std::cout << "index  " << itLoc->index[i] << std::endl;
						std::ifstream file1(itLoc->index[i]);
						if (file1.good())
						{
							std::cout << "exist\n";
							ok_200(client, itLoc->index[i]);
						}
						else
							i++;
					}
					if (i == itLoc->index.size())
					{
						std::cout << "not found \n";
						client.flagRed = true;
					}
				}
			}
			else
			{
				std::cout << "--------------is_file----------  " << std::endl;
				if (!itLoc->root.empty())
					file = itLoc->root + client.path;
				else if (!itSrv->root.empty())
					file = itLoc->root + client.path;
				else
					file = client.path;
				std::ifstream file1(file);
				if (file1.good())
				{
					std::cout << "exist\n";
					ok_200(client, file);
				}
				else
				{
					std::cout << "file not exist\n";
					err_404(client);
				}
			}
		}
	}
	else
	{
		std::cout << "405 Method Not Allowed\n";
		client.flagRed = true;
	}
	//////////////

	// std::deque<location>::iterator itLoc = itSrv->locations.begin();
	// std::string file;
	// while (itLoc != itSrv->locations.end())
	// {
	// 	if (itLoc->path_location == newPath)
	// 	{
	// 		while (!itLoc->allow_methods[i].empty())
	// 		{
	// 			if (itLoc->allow_methods[i] == "GET")
	// 				check++;
	// 			i++;
	// 		}
	// 		check++;
	// 		if (check != 2)
	// 		{
	// 			std::cout << "method not allow\n";
	// 			err_404(client);
	// 		}
	// 		file = itLoc->root + path;
	// 		std::cout << file << std::endl;
	// 		std::ifstream file1(file);
	// 		if (file1.good())
	// 		{
	// 			std::cout << "exist\n";
	// 			ok_200(client, file, path);
	// 		}
	// 		else
	// 		{
	// 			std::cout << "file not exist\n";
	// 			err_404(client);
	// 		}
	// 	}
	// 	itLoc++;
	// 	if (itLoc == itSrv->locations.end() && check == 0 && newPath != "/")
	// 	{
	// 		itLoc = itSrv->locations.begin();
	// 		newPath = newpath(newPath);
	// 	}
	// }
	// if (check == 0)
	// {
	// 	std::cout << "=========no location===============\n";
	// 	file = itSrv->root + path;
	// 	std::ifstream file1(file);
	// 	if (file1.good())
	// 	{
	// 		std::cout << "exist\n";
	// 		ok_200(client, file, path);
	// 	}
	// 	else
	// 	{
	// 		std::cout << "file not exist\n";
	// 		err_404(client);
	// 	}
	// }
}