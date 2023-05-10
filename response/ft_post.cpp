#include "ft_Response.hpp"


void ft_post(std::deque<location>::iterator itLoc, clients_info &client, std::map<std::string, std::string> err_pages)
{
	std::string file;
	time_t timee;
	if(client.map_request.find("Content-Type") == client.map_request.end())
		err_404(client, err_pages);
	else if (itLoc->auto_upload == "on")
	{
		if (client.end_header_req == 1)
		{
			timee = time(NULL);
			file = delSp(itLoc->upload_path + "/" + std::to_string(timee) + get_extension(client.map_request["Content-Type"].c_str()));

			client.fs.open(file , std::fstream::out | std::fstream::app);
			if (!client.fs.is_open())
				err_500(client, err_pages);
			client.flag_res = 0;
			client.end_header_req = 2;
		}
		if (client.post_finished)
		{
			if (client.body.size() >= client.content_len)
				client.fs << client.body;
			if (!itLoc->redirection.empty())
			{
				client.flagRed = true;
				ft_redi(itLoc->redirection, client);
			}
			else
				created_201(client, err_pages);
		}
	}
	else
	{
		err_403(client, err_pages);
	}
}