#include "ft_Response.hpp"
size_t get_len(clients_info &client)
{
	std::string ss;
	std::string line;
    while (std::getline(client.file, line)) 
	{
		ss.append(line);
		ss.append("\n");
	}
	return(ss.length() - ss.find("\r\n\r\n") - 5);
}

void statut_code(clients_info &client, std::map<std::string, std::string> err_pages, std::string err, std::string statut)
{
	if (client.flag_header == 0)
	{
		client.file.open(err_pages[err], std::ios::in | std::ios::binary | std::ios::ate);
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
							std::to_string(client.size) + "\r\n\r\n";

			send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		}
		client.flag_header = 1;
		client.send_hed = 1;
	}
}

void ok_200(clients_info &client, std::string file, std::map<std::string, std::string> err_pages)
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
			statut_code(client, err_pages, "403", "403 Forbidden");
		}
		client.header = "HTTP/1.1 200 OK\r\n"
						"Connection: close\r\n"
						"Content-Type: " +
						get_content_type(file.c_str()) + "\r\n"
														 "Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";
		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
		client.send_hed = 1;
	}
}

void ft_send(clients_info &client)
{
	std::string path;
	if (client.post_finished && client.method == "POST")
	{
		if (client.body.size() == client.content_len)
			client.fs << client.body;
		if (!client.itLoc->cgi_path.empty())
		{
			path = client.path;
			if (client.itLoc->path_location != "/")
				path.replace(0, client.itLoc->path_location.length(), client.itLoc->root);
			else
				path.replace(0, client.itLoc->path_location.length() - 1, client.itLoc->root);
			exec_cgi(client, path, client.err_client);
		}
		else
		{
			statut_code(client, client.err_client, "201", "201 created");
			client.flagRed = true;
		}
	}
	
	if (waitpid(client.pid, 0, WNOHANG) != 0 && !client.itLoc->cgi_path.empty() && client.send_hed == 0 && client.flag_ff == 0)
	{
		client.file.close();
		client.file.open(client.file_aa, std::ios::in);
		if (!client.file.is_open())
		{
			statut_code(client, client.err_client, "403", "413 Forbidden");
		}
		client.header = "HTTP/1.1 200 OK\r\n"
						"Connection: close\r\n"
						"Content-Length: " +
						std::to_string(get_len(client)) + "\r\n";
		client.file.close();
		client.file.open(client.file_aa, std::ios::in);
		client.send_hed = 1;
		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_ff = 1;
	}

	if (client.flag_ff == 1 || client.itLoc->cgi_path.empty() || client.send_hed)
	{
		client.file.read(client.response, MAX_SIZE);
		if (client.file.gcount())
		{
			send(client.socket_client_id, client.response, client.file.gcount(), 0);
			bzero(client.response, client.file.gcount());
		}
	}

	if (client.file.eof() || client.flagRed == true)
	{
		client.clear_client = true;
	}
}
