#include "ft_Response.hpp"

std::string get_content_type(std::string path)
{
	size_t p = path.find_last_of(".");
	std::string ext;
	if(p != std::string::npos)
	{
		ext = path.substr(p, path.length() - p);
		if (!ext.empty())
		{
			if (ext ==  ".css")
				return "text/css";
			if (ext ==  ".csv")
				return "text/csv";
			if (ext ==  ".gif")
				return "image/gif";
			if (ext ==  ".htm")
				return "text/html";
			if (ext ==  ".html")
				return "text/html";
			if (ext ==  ".ico")
				return "image/x-icon";
			if (ext ==  ".jpeg")
				return "image/jpeg";
			if (ext ==  ".jpg")
				return "image/jpeg";
			if (ext ==  ".js")
				return "application/javascript";
			if (ext ==  ".json")
				return "application/json";
			if (ext ==  ".png")
				return "image/png";
			if (ext ==  ".pdf")
				return "application/pdf";
			if (ext ==  ".svg")
				return "image/svg+xml";
			if (ext ==  ".txt")
				return "text/plain";
			if (ext ==  ".mp4")
				return "video/mp4";
		}

	}
	return "application/octet-stream";
}

std::string get_extension(std::string path)
{
	if (path == "text/css")
		return ".css";
	if (path == "text/csv")
		return ".csv";
	if (path == "image/gif")
		return ".gif";
	if (path == "text/htm")
		return ".html";
	if (path == "text/html")
		return ".html";
	if (path == "text/php")
		return ".php";
	if (path == "image/ico")
		return ".x-icon";
	if (path == "image/jpeg")
		return ".jpeg";
	if (path == "image/jpg")
		return ".jpeg";
	if (path == "application/js")
		return ".javascript";
	if (path == "application/json")
		return ".json";
	if (path == "image/png")
		return ".png";
	if (path == "application/pdf")
		return ".pdf";
	if (path == "image/svg")
		return ".svg+xml";
	if (path == "text/txt")
		return ".plain";
	if (path == "video/mp4")
		return ".mp4";
<<<<<<< HEAD
	if(strcmp(path, "plain/text") == 0)
		return(".txt");
=======
>>>>>>> 0442997b353148b0bc54230fecaf64f8e7180a3c
	return "";
}