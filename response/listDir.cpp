#include "ft_Response.hpp"

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
			if (client.itLoc->path_location == "/" && client.path == "/")
				output.append(client.path + ent->d_name);
			else
				output.append(client.path + "/" + ent->d_name);
			output.append("\">");
			output.append(ent->d_name);
			output.append("</a></li>");
		}
		closedir(dir);
	output.append("</ul></body></html>");
	client.header = "HTTP/1.1 200 OK\r\n"
					"Connection: close\r\n"
					"Content-Type: "
					"text/html\r\n"
					"Content-Length: " +
					ft_to_string(output.size()) +
					"\r\n\r\n";
		client.flagRed = true;
		if (send(client.socket_client_id, client.header.c_str(), client.header.size(), 0) <= 0)
			return;
		if (send(client.socket_client_id, output.c_str(), output.length(), 0) <= 0)
			return;
	}
	else
	{
		statut_code(client, "403", "403 Forbidden");
	}
}