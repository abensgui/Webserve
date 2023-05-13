#include "ft_Response.hpp"

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
			statut_code(client, err_pages, "403", "403 Forbidden");
		}
		client.header = "HTTP/1.1 " + statut +
						"\r\nConnection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
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
		std::cout << client.header << std::endl;
		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
		client.send_hed = 1;	}
}

void ft_send(clients_info &client)
{
	if (client.post_finished && client.method == "POST")
	{
		if (client.body.size() == client.content_len)
			client.fs << client.body;
		if (!client.itLoc->cgi_path.empty())
			exec_cgi(client,client.path.substr(1, client.path.length() - 1), client.err_client);
		else
		{
			statut_code(client, client.err_client, "201", "201 created");
			client.flagRed = true;
		}
	}
	if( waitpid(client.pid, 0, WNOHANG) != 0 && !client.itLoc->cgi_path.empty()  && client.send_hed == 0)
	{
		client.file.open(client.file_aa, std::ios::in);
			std::cout<< "file 413 " << client.file_aa<< std::endl;
		if (!client.file.is_open())
		{
			statut_code(client, client.err_client, "403", "413 Forbidden");
		}
		client.header = "HTTP/1.1 201 OK\r\nContent-Type: text/html\r\n";
		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_ff = 1;
	}
	
	if(client.flag_ff == 1 || client.itLoc->cgi_path.empty() || client.send_hed)
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
		std::cout << "client jfille eof \n";
		client.clear_client = true;
	}
}
