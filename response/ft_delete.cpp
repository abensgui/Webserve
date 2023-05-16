#include "ft_Response.hpp"

void ft_delete(clients_info &client)
{
	std::string file;
	file = client.path;
	if (client.itLoc->path_location != "/")
		file.replace(0, client.itLoc->path_location.length(), client.itLoc->root);
	else
		file.replace(0, client.itLoc->path_location.length() - 1, client.itLoc->root);
	file = delSp(file);
	std::cout<< "----------   " << file << std::endl;
	if (is_fileOrDir(client.path))
		statut_code(client, "403", "403 Forbidden");
	else
	{
		std::ifstream file1(file);
		if (file1.good())
		{
			if (std::remove(file.c_str()) == 0)
				statut_code(client,"200", "202 Accepted");
			else
				statut_code(client,  "403", "403 Forbidden");
		}
		else
			statut_code(client, "403", "404 Not Found");
	}
}