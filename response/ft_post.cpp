#include "ft_Response.hpp"


void ft_post(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages)
{
	std::string file;
	time_t timee;
	if(client.map_request.find("Content-Type") == client.map_request.end())
		statut_code(client, err_pages, "404", "404 Not Found");
	else if (itLoc->auto_upload == "on")
	{
		if (client.end_header_req == 1)
		{
			timee = time(NULL);
			file = delSp(itLoc->upload_path + "/" + std::to_string(timee) + get_extension(client.map_request["Content-Type"].c_str()));
			client.fs.open(file , std::fstream::out | std::fstream::app);
			if (!client.fs.is_open())
				statut_code(client, err_pages, "500", "Internal Server Error");
			client.flag_res = 0;
			client.end_header_req = 2;
		}
	}
	else
	{
		statut_code(client, err_pages, "403", "403 Forbidden");
	}
}