#include "ft_Response.hpp"
std::string delSp(std::string file)
{
	int p;
	while (1)
	{
		p = file.find("//");
		if (p != -1)
			file = file.substr(0, p) + file.substr(p + 1, file.length());
		else
			break;
	}
	return (file);
}

bool check_ext(std::string path)
{
	size_t p;
	if(path.find("?"))
		path = path.substr(0, path.find("?"));
	p = path.find_last_of(".");
	if(( p == path.length() - 4 &&  path.substr(p , 4) == ".php") || (p == path.length() - 3 &&  path.substr(p , 3) == ".py"))
	{
		return (true);
	}
	return (false);
}

void ft_get(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages)
{
	std::string file;
	std::string cookie;
	file = client.path;
	size_t i = 0;
	if (itLoc->path_location != "/")
		file.replace(0, itLoc->path_location.length(), itLoc->root);
	else
		file.replace(0, itLoc->path_location.length() - 1, itLoc->root);
	file = delSp(file);
	if (is_fileOrDir(file))
	{
		if (itLoc->auto_index == "on")
			listDir(client, file, itLoc);
		else
		{
			while (i < itLoc->index.size())
			{
				std::ifstream file1(itLoc->index[i]);
				if (file1.good())
				{
					if (!itLoc->cgi_path.empty())
						exec_cgi(client, file, err_pages);
					else
						ok_200(client, itLoc->index[i], err_pages);
				}
				else
					i++;
			}
			if (i == itLoc->index.size())
				statut_code(client, err_pages, "404", "404 Not Found");
		}
	}
	else
	{
		std::ifstream file1(file);
		if (!itLoc->cgi_path.empty() && check_ext(file))
		{
			std::cout << file << "---- cgi ---- \n";
			exec_cgi(client, file, err_pages);
		}
		else if (file1.good())
		{
				ok_200(client, file, err_pages);
		}
		else statut_code(client, err_pages, "404", "404 Not Found");
	}
}