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

void ft_get(clients_info &client)
{
	std::string file;
	std::string cookie;
	file = client.path;
	size_t i = 0;
	if (client.itLoc->path_location != "/")
		file.replace(0, client.itLoc->path_location.length(), client.itLoc->root);
	else
		file.replace(0, client.itLoc->path_location.length() - 1, client.itLoc->root);
	file = delSp(file);
	if (is_fileOrDir(file))
	{
		if (client.itLoc->auto_index == "on")
			listDir(client, file);
		else
		{
			if(client.itLoc->index.size() == 0)
				statut_code(client, "403", "403 Forbidden");
			else
			{
				while (i < client.itLoc->index.size())
				{
					std::ifstream file1(client.itLoc->index[i]);
					if (file1.good())
					{
						if (!client.itLoc->cgi_path.empty() && check_ext(file))
							exec_cgi(client, file);
						else
							ok_200(client, client.itLoc->index[i]);
						break;
					}
					i++;
				}
				if (i == client.itLoc->index.size())
					statut_code(client, "404", "404 Not Found");
			}
		}
	}
	else
	{
		std::ifstream file1(file);
		if (!client.itLoc->cgi_path.empty() && check_ext(file))
		{
			std::cout << file << "---- cgi ---- \n";
			exec_cgi(client, file);
		}
		else if (file1.good())
			ok_200(client, file);
		else if (access(file.c_str(), F_OK))
			statut_code(client, "404", "404 Not Found");
		else
			statut_code(client,  "403", "403 Forbidden");
	}
}