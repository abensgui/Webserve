#include "ft_Response.hpp"

std::string get_content_type(const char *path)
{
	const char *last_dot = strrchr(path, '.');
	if (last_dot)
	{
		if (strcmp(last_dot, ".css") == 0)
			return "text/css";
		if (strcmp(last_dot, ".csv") == 0)
			return "text/csv";
		if (strcmp(last_dot, ".gif") == 0)
			return "image/gif";
		if (strcmp(last_dot, ".htm") == 0)
			return "text/html";
		if (strcmp(last_dot, ".html") == 0)
			return "text/html";
		if (strcmp(last_dot, ".ico") == 0)
			return "image/x-icon";
		if (strcmp(last_dot, ".jpeg") == 0)
			return "image/jpeg";
		if (strcmp(last_dot, ".jpg") == 0)
			return "image/jpeg";
		if (strcmp(last_dot, ".js") == 0)
			return "application/javascript";
		if (strcmp(last_dot, ".json") == 0)
			return "application/json";
		if (strcmp(last_dot, ".png") == 0)
			return "image/png";
		if (strcmp(last_dot, ".pdf") == 0)
			return "application/pdf";
		if (strcmp(last_dot, ".svg") == 0)
			return "image/svg+xml";
		if (strcmp(last_dot, ".txt") == 0)
			return "text/plain";
		if (strcmp(last_dot, ".mp4") == 0)
			return "video/mp4";
	}
	return "application/octet-stream";
}

std::string get_extension(const char *path)
{
	if (strcmp(path, "text/css") == 0)
		return ".css";
	if (strcmp(path, "text/csv") == 0)
		return ".csv";
	if (strcmp(path, "image/gif") == 0)
		return ".gif";
	if (strcmp(path, "text/htm") == 0)
		return ".html";
	if (strcmp(path, "text/html") == 0)
		return ".html";
	if (strcmp(path, "text/php") == 0)
		return ".php";
	if (strcmp(path, "image/ico") == 0)
		return ".x-icon";
	if (strcmp(path, "image/jpeg") == 0)
		return ".jpeg";
	if (strcmp(path, "image/jpg") == 0)
		return ".jpeg";
	if (strcmp(path, "application/js") == 0)
		return ".javascript";
	if (strcmp(path, "application/json") == 0)
		return ".json";
	if (strcmp(path, "image/png") == 0)
		return ".png";
	if (strcmp(path, "application/pdf") == 0)
		return ".pdf";
	if (strcmp(path, "image/svg") == 0)
		return ".svg+xml";
	if (strcmp(path, "text/txt") == 0)
		return ".plain";
	if (strcmp(path, "video/mp4") == 0)
		return ".mp4";
	return "application/octet-stream";
}