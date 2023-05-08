#include "ft_Response.hpp"

void ft_delete(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages)
{
	std::string file;
	if (!itLoc->redirection.empty())
	{
		client.flagRed = true;
		ft_redi(itLoc->redirection, client);
	}
	else
	{
		if (itLoc->path_location != "/")
			file.replace(0, itLoc->path_location.length(), itLoc->root);
		else
			file.replace(0, itLoc->path_location.length() - 1, itLoc->root);
		file = delSp(file);
		if (is_fileOrDir(client.path))
			err_403(client, err_pages);
		else
		{
			std::ifstream file1(file);
			if (file1.good())
			{
				if (std::remove(file.c_str()) == 0)
					ft_202(client, err_pages);
				else
					err_403(client, err_pages);
			}
			else
				err_404(client, err_pages);
		}
	}
}