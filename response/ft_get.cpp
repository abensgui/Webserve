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

void exec_cgi(clients_info &client, std::string file, std::map<std::string, std::string> err_pages)
{

	int fd;
	int fd1;
	int pid = fork();
	time_t timee;
	timee = time(NULL);
	std::string name = "/tmp/aa" + std::to_string(timee);
	if (client.flag_header == 0)
	{
		if (pid == 0)
		{
			fd = open(file.c_str(), O_RDONLY);
			fd1 = open(name.c_str(), O_CREAT | O_RDWR, 0777);
			std::cout << "  " << fd << " " << fd1 << std::endl;
			std::cout << "open : " << fd1 << "\n";
			dup2(fd, 0);
			dup2(fd1, 1);
			close(fd);
			close(fd1);
			char *args[] = {(char *)"./response/php-cgi", NULL};
			execve(args[0], args, NULL);
			std::cout << "execve\n";
		}
		wait(NULL);
		// client.path_file = file;
		client.file.open(name, std::ios::in);
		// client.file.seekg(0, std::ios::end);
		// client.size = client.file.tellg();
		// client.file.seekg(0, std::ios::beg);
		if (!client.file.is_open())
		{
			statut_code(client, err_pages, "403", "403 Forbidden");
		}
		client.header = "HTTP/1.1 200 OK\r\n";
		send(client.socket_client_id, client.header.c_str(), client.header.size(), 0);
		client.flag_header = 1;
	}
}

void ft_get(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages)
{
	std::string file;
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
					ok_200(client, itLoc->index[i], err_pages);
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
		if (file1.good())
		{
			if (!itLoc->cgi_path.empty())
				exec_cgi(client, file, err_pages);
			else
				ok_200(client, file, err_pages);
		}
		else
			statut_code(client, err_pages, "404", "404 Not Found");
	}
}