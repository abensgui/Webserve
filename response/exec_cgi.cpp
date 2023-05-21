#include "ft_Response.hpp"

std::deque<std::string> get_env(clients_info &client, std::string file)
{
	std::deque<std::string> env;
	env.push_back("REQUEST_METHOD=" + client.method);
	env.push_back("SCRIPT_FILENAME=" + file.substr(0, file.find("?")));
	env.push_back("CONTENT_LENGTH=" + ft_to_string(client.content_len));
	env.push_back("CONTENT_TYPE=" + client.map_request["Content-Type"]);
	env.push_back("REDIRECT_STATUS=200");
	env.push_back("HTTP_COOKIE=" + client.map_request["Cookie"]);
	if (file.find("?") != std::string::npos)
	{
		env.push_back("QUERY_STRING=" + file.substr(file.find("?") + 1, file.length() - file.find("?")));
	}
	else
		env.push_back("QUERY_STRING=");
	return (env);
}

void exec_cgi(clients_info &client, std::string file)
{

	if (client.flag_header == 0)
	{
		int fd;
		int fd1;
		time_t timee;
		timee = time(NULL);
		size_t i = 0;

		std::deque<std::string> envd;
		envd = get_env(client, file);
		char *env[envd.size() + 1];
		while (i < envd.size())
		{
			env[i] = (char *)envd[i].c_str();
			i++;
		}
		env[i] = NULL;

		if (file.find("?"))
			file = file.substr(0, file.find("?"));

		client.fs.close();
		client.file_aa = "/tmp/res" + ft_to_string(timee);
		client.pid = fork();
		if (client.pid == 0)
		{
			if (client.method == "GET")
			{
				fd = open(file.c_str(), O_RDONLY);
				fd1 = open(client.file_aa.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
			}
			else
			{

				fd = open(client.post_file.c_str(), O_RDONLY);
				fd1 = open(client.file_aa.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
			}
			dup2(fd, 0);
			dup2(fd1, 1);
			close(fd);
			close(fd1);
			char *args[] = {(char *)client.itLoc->cgi_path.c_str(), (char *)file.c_str(), NULL};
			execve(args[0], args, env);
			exit(1);
		}
		client.flag_header = 1;
		// remove(client.post_file.c_str());
	}
	
}