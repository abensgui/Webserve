#include "ft_Response.hpp"
void default_err(clients_info &client, std::string status)
{
    std::string output;
    output.append("<html><body><div>");
    output.append(status);
    output.append("</div></body</body></html>");
    client.header = "HTTP/1.1 " + status +
                    "\r\nConnection: close\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: " +
                    ft_to_string(output.length()) +
                    "\r\n\r\n";
    if (send(client.socket_client_id, client.header.c_str(), client.header.size(), 0) <= 0)
    {
        client.flagRed = true;
        return;
    }
    if (send(client.socket_client_id, output.c_str(), output.length(), 0) <= 0)
    {
        client.flagRed = true;
        return;
    }
    client.flagRed = true;

}