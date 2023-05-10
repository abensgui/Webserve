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
		client.header = "HTTP/1.1 "
						+ statut +
						"\r\nConnection: close\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " +
						std::to_string(client.size) + "\r\n\r\n";

		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}
}

void ft_send(clients_info &client)
{
	// std::cout << "dsfds   "<< client.post_finished << std::endl;
	if (client.post_finished)
	{
		std::cout << "here\n";
		if (client.body.size() == client.content_len)
			client.fs << client.body;
		statut_code(client, client.err_client, "201", "201 created");
	}
	client.file.read(client.response, MAX_SIZE);
	if (client.file.gcount())
	{
		send(client.socket_client_id, client.response, client.file.gcount(), 0);
		bzero(client.response,client.file.gcount());
	}
	if (client.file.eof() || client.flagRed == true)
	{
		std::cout << "client jfille eof \n";
		client.clear_client = true;
	}
}

// void err_403(clients_info &client, std::map<std::string, std::string> err_pages)
// {
// 	if (client.flag_header == 0)
// 	{
// 		client.file.open(err_pages["403"], std::ios::in | std::ios::binary | std::ios::ate);
// 		client.file.seekg(0, std::ios::end);
// 		client.size = client.file.tellg();
// 		client.file.seekg(0, std::ios::beg);
// 		if (!client.file.is_open())
// 			return;
// 		client.header = "HTTP/1.1 403 Forbidden\r\n"
// 						"Connection: close\r\n"
// 						"Content-Type: text/html\r\n"
// 						"Content-Length: " +
// 						std::to_string(client.size) + "\r\n\r\n";
// 		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
// 		client.flag_header = 1;
// 	}
// 	ft_send(client);
// }

// void err_500(clients_info &client, std::map<std::string, std::string> err_pages)
// {
// 	if (client.flag_header == 0)
// 	{
// 		client.file.open(err_pages["500"], std::ios::in | std::ios::binary | std::ios::ate);
// 		client.file.seekg(0, std::ios::end);
// 		client.size = client.file.tellg();
// 		client.file.seekg(0, std::ios::beg);
// 		if (!client.file.is_open())
// 		{
// 			err_403(client, err_pages);
// 			return;
// 		}
// 		client.header = "HTTP/1.1 500 Internal Server Error\r\n"
// 						"Connecton: close\r\n"
// 						"Content-Type: text/html\r\n"
// 						"Content-Length: " +
// 						std::to_string(client.size) + "\r\n\r\n";

// 		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
// 		client.flag_header = 1;
// 	}
// 	ft_send(client);
// }

// void err_405(clients_info &client, std::map<std::string, std::string> err_pages)
// {
// 	if (client.flag_header == 0)
// 	{
// 		client.file.open(err_pages["405"], std::ios::in | std::ios::binary | std::ios::ate);
// 		if (!client.file.is_open())
// 		{
// 			err_403(client, err_pages);
// 			return;
// 		}
// 		client.file.seekg(0, std::ios::end);
// 		client.size = client.file.tellg();
// 		client.file.seekg(0, std::ios::beg);
// 		client.header = "HTTP/1.1 405 Method Not Allowed\r\n"
// 						"Connection: close\r\n"
// 						"Content-Type: text/html\r\n"
// 						"Content-Length: " +
// 						std::to_string(client.size) + "\r\n\r\n";

// 		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
// 		client.flag_header = 1;
// 	}
// 	ft_send(client);
// }

// void err_404(clients_info &client, std::map<std::string, std::string> err_pages)
// {
// 	if (client.flag_header == 0)
// 	{
// 		client.file.open(err_pages["404"], std::ios::in | std::ios::binary | std::ios::ate);
// 		client.file.seekg(0, std::ios::end);
// 		client.size = client.file.tellg();
// 		client.file.seekg(0, std::ios::beg);

// 		if (!client.file.is_open())
// 		{
// 			err_403(client, err_pages);
// 			return;
// 		}
// 		client.header = "HTTP/1.1 404 Not Found\r\n"
// 						"Connection: close\r\n"
// 						"Content-Type: text/html\r\n"
// 						"Content-Length: " +
// 						std::to_string(client.size) + "\r\n\r\n";

// 		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
// 		client.flag_header = 1;
// 	}
// 	ft_send(client);
// }


// void created_201(clients_info &client, std::map<std::string, std::string> err_pages)
// {
// 	std::cout << "----------201--------\n";
// 	if (client.flag_header == 0)
// 	{
// 		client.file.open(err_pages["201"], std::ios::in | std::ios::binary | std::ios::ate);
// 		client.file.seekg(0, std::ios::end);
// 		client.size = client.file.tellg();
// 		client.file.seekg(0, std::ios::beg);

// 		if (!client.file.is_open())
// 		{
// 			std::cout << "----------403--------\n";
// 			err_403(client, err_pages);
// 			return;
// 		}
// 		client.header = "HTTP/1.1 201 created\r\n"
// 						"Connection: close\r\n"
// 						"Content-Type: text/html\r\n"
// 						"Content-Length: " +
// 						std::to_string(client.size) + "\r\n\r\n";
// 		std::cout << client.header << std::endl;
// 		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
// 		client.flag_header = 1;
// 	}
// 	ft_send(client);
// }

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
	}
}