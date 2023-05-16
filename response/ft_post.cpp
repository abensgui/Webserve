#include "ft_Response.hpp"

void ft_post(clients_info &client)
{
	std::string file;
	time_t timee;
	if (client.map_request.find("Content-Type") == client.map_request.end())
	{
		std::cout << "inside code status 404 \n";
		statut_code(client, "404", "404 Not Found");
	}
	else if (client.itLoc->auto_upload == "on")
	{
		if (client.end_header_req == 1)
		{
			timee = time(NULL);
<<<<<<< HEAD
			file = delSp(itLoc->upload_path + "/" + std::to_string(timee) + get_extension(client.map_request["Content-Type"].c_str()));
=======
			file = delSp(client.itLoc->upload_path + "/" + std::to_string(timee) + get_extension(client.map_request["Content-Type"].c_str()));
>>>>>>> 0442997b353148b0bc54230fecaf64f8e7180a3c
			client.post_file = file;
			std::cout << "----------------------------------  "<<file <<std::endl;			
			client.fs.open(file, std::fstream::out);
			if (!client.fs.is_open())
			{
				statut_code(client, "500", "Internal Server Error");
			}
			client.end_header_req = 2;
		}
	}
	else
	{
		statut_code(client, "403", "403 Forbidden");
	}
}