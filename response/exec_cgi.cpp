#include "ft_Response.hpp"

std::deque<std::string> get_env(clients_info &client, std::string file)
{
	std::deque<std::string> env;
	env.push_back("REQUEST_METHOD=" + client.method);
	env.push_back("SCRIPT_FILENAME=" + file);
	env.push_back("CONTENT_LENGTH=" + std::to_string(client.content_len));
	env.push_back("CONTENT_TYPE=" + client.map_request["Content-Type"]);
	env.push_back("REDIRECT_STATUS=200");
	
	// if(file.find("?"))
	// 	env.push_back("QUERY_STRING=" + file.substr(file.find("?") + 1, file.length() - file.find("?")));
	return (env);
}

void exec_cgi(clients_info &client, std::string file, std::map<std::string, std::string> err_pages)
{

	if (client.flag_header == 0)
	{
		(void)err_pages;
		int fd;
		int fd1;
		time_t timee;
		timee = time(NULL);
		size_t i = 0;
		////////////////////////////////////
		std::deque<std::string> envd;
		envd = get_env(client, file);

		char *env[envd.size() + 1];
		while (i < envd.size())
		{
			env[i] = (char *)envd[i].c_str();
			i++;
		}
		env[i] = NULL;

		client.fs.close();
		client.file_aa = "trach/res" + std::to_string(timee);
		std::cout << "method : " << env[0]<< std::endl;
				std::cout<< client.flag_header<< "   =======TRACH FILE----////// " << client.file_aa << std::endl;
		client.pid = fork();
			if (client.pid == 0)
			{
				if (client.method == "GET")
				{
					fd = open(file.c_str(), O_RDONLY);
					fd1 = open(client.file_aa.c_str(), O_CREAT | O_RDWR, 0777);
				}
				else
				{
					fd = open(client.post_file.c_str(), O_RDONLY);
					std::cout <<"post_file :: " << client.post_file << std::endl;
					fd1 = open(client.file_aa.c_str(), O_CREAT | O_RDWR, 0777);
				}
				dup2(fd, 0);
				dup2(fd1, 1);
				close(fd);
				close(fd1);
				char *args[] = {(char *)client.itLoc->cgi_path.c_str(), (char *)file.c_str(), NULL};
				execve(args[0], args, env);
		}
		
		client.flag_header = 1;
	}
}