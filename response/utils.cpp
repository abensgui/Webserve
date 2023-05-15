#include "ft_Response.hpp"

bool err_hed_req(clients_info &client)
{
    int per = client.path.find("%");
	if (client.method == "POST" && \
		client.map_request.find("Transfer-Encoding") != client.map_request.end() && \
		client.map_request["Transfer-Encoding"] != "chunked")
	{
		client.is_post = 1;
		client.exit_status.first = "501";
		client.exit_status.second = "501 Not Implemented";
	}
	else if (client.method == "POST" && client.map_request.find("Content-Length") == client.map_request.end() &&
		client.map_request.find("Transfer-Encoding") == client.map_request.end())
	{
		client.is_post = 1;
		client.exit_status.first = "400";
		client.exit_status.second = "400 Bad Request";
	}
	else if (client.path.size() > 2048)
	{
		client.exit_status.first = "414";
		client.exit_status.second = "414 URI Too Long";
	}
	else if (per != -1)
	{
		client.exit_status.first = "400";
		client.exit_status.second = "400 Bad Request";
	}
}