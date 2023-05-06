#include "ft_Response.hpp"
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

void ft_202(clients_info &client)
{

	client.file.open("error/403.html", std::ios::in | std::ios::binary | std::ios::ate);
	client.file.seekg(0, std::ios::end);
	client.size = client.file.tellg();
	client.file.seekg(0, std::ios::beg);
	if (!client.file.is_open())
	{
		// default err
		//  err_403(client);
		return;
	}
	client.header = "HTTP/1.1 202 Accepted\r\n\r\n";

	send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
	client.clear_client = 1;
}

void err_403(clients_info &client)
{
	if (client.flag_header == 0)
	{
		client.file.open("error/403.html", std::ios::in | std::ios::binary | std::ios::ate);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);
		if (!client.file.is_open())
		{
			// default err
			//  err_403(client);
			return;
		}
		client.header = "HTTP/1.1 403 Forbidden\r\n"
						"Connection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		//std::cout << "vvvvv\n";
		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
		//std::cout << "vvvvv\n";
	}

	//std::cout << "SEND BF\n";
	client.file.read(client.response, MAX_SIZE);
	//std::cout << "SEND AF\n";
	send(client.socket_client_id, client.response, MAX_SIZE, 0);
	bzero(client.response, MAX_SIZE);
}

void err_500(clients_info &client)
{
	if (client.flag_header == 0)
	{
		if (!client.file.is_open())
		{
			err_403(client);
			return;
		}
		client.file.open("error/500.html", std::ios::in | std::ios::binary | std::ios::ate);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);
		client.header = "HTTP/1.1 500 Internal Server Error\r\n"
						"Connection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}

	client.file.read(client.response, MAX_SIZE);
	send(client.socket_client_id, client.response, MAX_SIZE, 0);
	bzero(client.response, MAX_SIZE);
}

void err_405(clients_info &client)
{
	if (client.flag_header == 0)
	{
		client.file.open("error/405.html", std::ios::in | std::ios::binary | std::ios::ate);
		if (!client.file.is_open())
		{
			err_403(client);
			return;
		}
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);
		client.header = "HTTP/1.1 405 Method Not Allowed\r\n"
						"Connection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}

	client.file.read(client.response, MAX_SIZE);
	send(client.socket_client_id, client.response, MAX_SIZE, 0);
	bzero(client.response, MAX_SIZE);
}

void err_404(clients_info &client)
{
	if (client.flag_header == 0)
	{
		//std::cout << "aghfsdhgafshd\n";

		client.file.open("error/404.html", std::ios::in | std::ios::binary | std::ios::ate);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);

		if (!client.file.is_open())
		{
			err_403(client);
			return;
		}
		client.header = "HTTP/1.1 404 Not Found\r\n"
						"Connection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " + std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}

	client.file.read(client.response, MAX_SIZE);
	if (client.file.gcount())
		send(client.socket_client_id, client.response, client.file.gcount(), 0);
	bzero(client.response, MAX_SIZE);
}

void ok_200(clients_info &client, std::string file)
{
	if (client.flag_header == 0)
	{
	//std::cout << "good " << file << std::endl;
		client.path_file = file;
		client.file.open(file, std::ios::in | std::ios::binary | std::ios::ate);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);

		if (!client.file.is_open())
		{
			//std::cout << "----------403--------\n";
			err_403(client);
			return;
		}
		client.header = "HTTP/1.1 200 OK\r\n"
						"Connection: close\r\n"
						"Content-Type: " + get_content_type(file.c_str()) + "\r\n"
						"Content-Length: " + std::to_string(client.size) + "\r\n\r\n";
		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}

	client.file.read(client.response, MAX_SIZE);
	if (client.file.gcount())
		send(client.socket_client_id, client.response, client.file.gcount(), 0);
	bzero(client.response, sizeof(client.response));
	//std::cout << "good " << file << std::endl;

}

void created_201(clients_info &client)
{
	if (client.flag_header == 0)
	{
		client.header = "HTTP/1.1 201 created\r\n"
						"Connection: close\r\n"
						"Content-Type: " +
						client.map_request["Content-Type"] + "\r\n"
															 "Content-Length: 0" +
						"\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}
}

void listDir(clients_info &client, std::string file)
{
	std::string output;
	output.append("<html><body><ul>");

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(file.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			output.append("<li><a href=\"");
			output.append(ent->d_name);
			output.append("\">");
			output.append(ent->d_name);
			output.append("</a></li>");
		}
		closedir(dir);
	}
	else
	{
		perror("Unable to open directory");
	}
	output.append("</ul></body></html>");
	client.header = "HTTP/1.1 200 OK\r\n"
					"Connection: close\r\n"
					"Content-Type: "
					"text/html\r\n"
					"Content-Length: " +
					std::to_string(output.size()) +
					"\r\n\r\n";
	send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
	send(client.socket_client_id, output.c_str(), output.length(), 0);
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
	//std::cout << "==========here" << std::endl;
	client.header = "HTTP/1.1 301 Moved Permanently\r\n"
					"Location: " +
					redi + "\r\n\n\r";
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
			//std::cout << path << " is a directory" << std::endl;
			return (1);
		}
	}
	return (0);
}

void ft_get(std::deque<location>::iterator itLoc, clients_info &client)
{
	std::string file;
	file = client.path;
	size_t i = 0;
	if (!itLoc->redirection.empty())
	{
		//std::cout << "-------------rdi------------\n";
		client.flagRed = true;
		ft_redi(itLoc->redirection, client);
	}
	else
	{
		// std::cout << "--------------is_file----------  "<< itLoc->path_location << std::endl;
		file.replace(0, itLoc->path_location.length() - 1, itLoc->root);
		// std::cout << "--------------file---------- ::: " << file << std::endl;
		if (is_fileOrDir(file))
		{

			if (itLoc->auto_index == "on")
				listDir(client, file);
			else
			{
				while (i < itLoc->index.size())
				{
					//std::cout << "index  " << itLoc->index[i] << std::endl;
					std::ifstream file1(itLoc->index[i]);
					if (file1.good())
					{
						//std::cout << "exist\n";
						ok_200(client, itLoc->index[i]);
					}
					else
						i++;
				}
				if (i == itLoc->index.size())
					err_404(client);
			}
		}
		else
		{

			std::ifstream file1(file);
			if (file1.good())
			{
				//std::cout << "exist11111\n";
				ok_200(client, file);
				//std::cout << "exist11111\n";
			}
			else
			{
				//std::cout << "file not exist\n";
				err_404(client);
			}
		}
	}
}

void ft_post(std::deque<location>::iterator itLoc, clients_info &client)
{
	if (itLoc->auto_upload == "on")
	{
		if (client.end_header_req == 1)
		{
			client.fs.open(itLoc->upload_path + "/" + client.path , std::fstream::out | std::fstream::app);
			if (!client.fs.is_open())
			{
				//std::cout << "----------500--------\n";
			}
			client.flag_res = 0;
			client.end_header_req = 2;
		}
		if (client.post_finished)
		{
			if (!itLoc->redirection.empty())
			{
				//std::cout << "-------------rdi------------\n";
				client.flagRed = true;
				ft_redi(itLoc->redirection, client);
			}
			else
			{
				created_201(client);
				client.flagRed = true;
			}
		}
	}
	else
	{
		//std::cout << "403 Forbidden\n";
		// char aa[] = "HTTP/1.1 200 OK\r\nDate: Wed, 21 Oct 2015 07:28:00 GMT\r\n";
		// send(client.socket_client_id, aa, sizeof(aa), 0);
		// client.clear_client = true;
		err_403(client);
	}
}

void ft_delete(std::deque<location>::iterator itLoc, clients_info &client)
{
	std::string file;
	if (!itLoc->redirection.empty())
	{
		//std::cout << "-------------rdi------------\n";
		client.flagRed = true;
		ft_redi(itLoc->redirection, client);
	}
	else
	{
		if (is_fileOrDir(client.path))
			err_404(client);
		else
		{

			//std::cout << "--------------is_file----------  " << std::endl;
			if (!itLoc->root.empty())
			{
				if (itLoc->path_location != "/")
					file = client.filename_post.replace(0, itLoc->path_location.length(), itLoc->root);
				else
					file = itLoc->root + "/" + client.filename_post;
			}
			else
				file = client.filename_post;
			//std::cout << "--------------file---------- ::: " << file << std::endl;
			std::ifstream file1(file);
			if (file1.good())
			{
				if (std::remove(file.c_str()) == 0)
				{
					ft_202(client);
				}
				else
				{
					std::perror("Error deleting file");
				}
			}
			else
			{
				//std::cout << "file not exist\n";
				err_404(client);
			}
		}
	}
}

void ft_Response(std::deque<server> &Srv, clients_info &client)
{
	std::deque<location>::iterator itLoc;
	std::deque<server>::iterator itSrv;

	itSrv = select_server(Srv, client);
	if (itSrv == Srv.end())
		itSrv = Srv.begin();
	itLoc = location_match(itSrv->locations, client.path);

	if (itLoc == itSrv->locations.end())
		err_404(client);
	else
	{
		if (methodAllow(client.method, itLoc->allow_methods))
		{
			if (client.method == "GET")
				ft_get(itLoc, client);
			else if (client.method == "POST")
			{
				// //std::cout << "post\n";

				ft_post(itLoc, client);
			}
			else if (client.method == "DELETE")
			{
				//std::cout << "DELETE\n";
				//std::cout << client.filename_post << "|\n";
				ft_delete(itLoc, client);
			}
		}
		else
		{
			err_405(client);
		}
	}
	if (client.file.eof() || client.flagRed == true)
	{
		//std::cout << "client jfille eof \n";
		client.clear_client = true;
	}
}