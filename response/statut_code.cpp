#include "ft_Response.hpp"
void get_len(clients_info &client)
{
	std::string ss;
	std::string line;
	std::string status;
	size_t size_hed = 0;
	while (std::getline(client.file, line))
	{	
		int pos = line.find("Status:");
		if(pos != -1)
		{
			line = line.substr(0,  line.size() - 1);
			status = line.substr(pos + 7);
			size_hed = line.length() + 2;	
		}
		else
		{
			ss.append(line);
			ss.append("\n");
		}
		if(ss.find("\r\n\r\n") != std::string::npos || line.empty())
			break;
		line.clear();
	}

	size_hed += ss.length();
	if(ss.find("Content-type:") == std::string::npos && ss.find("Content-Type:") == std::string::npos)
		ss += "Content-Type: text/html\r\n";
		
	client.header =	"HTTP/1.1 200 OK\r\n"
					"Content-Length: " + ft_to_string(client.size - size_hed) +
					"\r\n" + ss;
	if(!status.empty())
	{
		client.header.replace(9, 6, status);
	}
}


void statut_code(clients_info &client, std::string err, std::string statut)
{
	if (client.flag_header == 0)
	{
		client.file.open(client.itSrv->err_pages[err], std::ios::in | std::ios::binary | std::ios::ate);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);
		if (!client.file.is_open())
		{
			default_err(client, statut);
		}
		else
		{
			client.header = "HTTP/1.1 " + statut +
							"\r\nConnection: close\r\n"
							"Content-Type: text/html\r\n"
							"Content-Length: " +
							ft_to_string(client.size) + "\r\n\r\n";

			if (send(client.socket_client_id, client.header.c_str(), client.header.size(), 0) <= 0)
			{
				client.clear_client = true;
				return;
			}
		}
		client.flag_header = 1;
		client.send_hed = 1;
	}
}

void ok_200(clients_info &client, std::string file)
{
	if (client.flag_header == 0)
	{
		client.path_file = file;
		client.file.open(file, std::ios::in | std::ios::binary | std::ios::ate);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);

		if (!client.file.is_open())
		{
			statut_code(client, "403", "403 Forbidden");
		}
		client.header = "HTTP/1.1 200 OK\r\n"
						"Connection: close\r\n"
						"Content-Type: " +
						get_content_type(file) + "\r\n"
												 "Content-Length: " +
						ft_to_string(client.size) + "\r\n\r\n";
		if (send(client.socket_client_id, client.header.c_str(), client.header.size(), 0) <= 0)
		{
			client.clear_client = true;
			return;
		}
		client.flag_header = 1;
		client.send_hed = 1;
	}
}

void ft_send(clients_info &client)
{
	std::string path;
	if (client.post_finished && client.method == "POST")
	{
		if (client.body.size() == client.content_len || client.is_chunk)
			client.fs << client.body;
		if (!client.itLoc->cgi_path.empty())
		{
			path = client.path;
			if (client.itLoc->path_location != "/")
				path.replace(0, client.itLoc->path_location.length(), client.itLoc->root);
			else
				path.replace(0, client.itLoc->path_location.length() - 1, client.itLoc->root);
			exec_cgi(client, path);
		}
		else
		{
			statut_code(client, "201", "201 created");
			client.flagRed = true;
		}
	}
	if (waitpid(client.pid, 0, WNOHANG) != 0 && !client.itLoc->cgi_path.empty() && client.send_hed == 0 && client.flag_ff == 0)
	{
		client.file.close();
		client.file.open(client.file_aa, std::ios::in);
		client.file.seekg(0, std::ios::end);
		client.size = client.file.tellg();
		client.file.seekg(0, std::ios::beg);
		if (!client.file.is_open())
		{
			statut_code(client, "403", "413 Forbidden");
		}
		get_len(client);
		client.send_hed = 1;
		if (send(client.socket_client_id, client.header.c_str(), client.header.size(), 0) <= 0)
		{
			client.clear_client = true;
			return;
		}
		client.flag_ff = 1;
	}

	if (client.flag_ff == 1 || client.itLoc->cgi_path.empty() || client.send_hed)
	{
		client.file.read(client.response, MAX_SIZE);
		// std::cout << "HERE CLIENT : " << client.socket_client_id << "\n";
		if (client.file.gcount())
		{
			// std::cout << "BEFORE : " << "\n";
			if (send(client.socket_client_id, client.response, client.file.gcount(), 0) <= 0)
			{
				client.clear_client = true;
				return;
			}
			// std::cout << "AFTER : " << "\n";
			bzero(client.response, client.file.gcount());
		}
	}

	if (client.file.eof() || client.flagRed == true)
	{
		// std::cout << "FINISH : " << "\n";
		client.clear_client = true;
	}
}
