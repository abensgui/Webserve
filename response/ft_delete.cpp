#include "ft_Response.hpp"

void ft_delete(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages)
{
	std::string file;
	file = client.path;
	if (itLoc->path_location != "/")
		file.replace(0, itLoc->path_location.length(), itLoc->root);
	else
		file.replace(0, itLoc->path_location.length() - 1, itLoc->root);
	file = delSp(file);
	std::cout<< "----------   " << file << std::endl;
	if (is_fileOrDir(client.path))
		statut_code(client, err_pages, "403", "403 Forbidden");
	else
	{
		std::ifstream file1(file);
		if (file1.good())
		{
			if (std::remove(file.c_str()) == 0)
				statut_code(client, err_pages, "200", "202 Accepted");
			else
				statut_code(client, err_pages, "403", "403 Forbidden");
		}
		else
			statut_code(client, err_pages, "403", "404 Not Found");
	}
}