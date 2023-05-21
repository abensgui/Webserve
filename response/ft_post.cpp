#include "ft_Response.hpp"

void ft_post(clients_info &client)
{
	std::string file;
	time_t timee;
	if (client.itLoc->auto_upload == "on")
	{
		if (client.end_header_req == 1)
		{
			timee = time(NULL);
			file = delSp(client.itLoc->upload_path + "/" + ft_to_string(timee) + get_extension(client.map_request["Content-Type"].c_str()));
			client.post_file = file;
			client.fs.open(file, std::fstream::out);
			if (!client.fs.is_open())
			{
				statut_code(client, "500", "500 Internal Server Error");
			}
			client.end_header_req = 2;
		}
	}
	else
	{
		statut_code(client, "403", "403 Forbidden");
	}
}