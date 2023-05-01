#include "configuration.hpp"

void    confi::split_string(std::string &str, location &lc, char c)
{
    char *word = strtok(const_cast<char *>(str.c_str()), " ");

    word = strtok(NULL, " ");
    while (word)
    {
        if (c == 'm' && (!strcmp(word, "GET") || !strcmp(word, "POST") || !strcmp(word, "DELETE")))
            lc.allow_methods.push_back(word);
        else if (c == 'i')
            lc.index.push_back(word);
        else
        {
            std::cerr << "You must Follow Our Rules in Methods\n";
            exit(1);
        }
        word = strtok(NULL, " ");
    }
}

int     confi::setup_configuration(std::string file)
{
    std::fstream congi_file;
    std::string line, key, value;
    struct server sr;
    struct location lc;
    int i = -1;
    int j = -1;
    congi_file.open(file);
    if (congi_file.is_open())
    {
        while (getline(congi_file, line))
        {
            i = -1;
            if (!line.compare("server"))
            {
                i = 0;
                j = -1;
                while (getline(congi_file, line))
                {
                    std::stringstream strm(line);
                    strm >> key;
                    strm >> value;
                    if (!line.compare("{"))
                        i++;
                    else if (!key.compare("location"))
                    {
                        j = 0;
                        if (!strm.eof())
                        {
                            std::cerr << "Error : location Path\n";
                            exit(1);
                        }
                        lc.path_location = value;
                        lc.auto_index = "on";
                        // lc.redirection = "";
                        while (getline(congi_file, line))
                        {
                            std::string key_local, value_local;
                            std::stringstream strm_local(line);
                            strm_local >> key_local;
                            strm_local >> value_local;
                            if (!line.compare("{"))
                            {
                                j++;
                                continue;
                            }
                            else if (!line.compare("}"))
                            {
                                j--;
                                sr.locations.push_back(lc);
                                lc.allow_methods.clear();
                                lc.index.clear();
                                lc.path_location.clear();
                                lc.root.clear();
                                lc.upload_pass.clear();
                                lc.auto_index.clear();
                                lc.redirection.clear();
                                break;
                            }
                            if (!key_local.compare("allow_methods"))
                                split_string(line, lc, 'm');
                            else if (!key_local.compare("index"))
                                split_string(line, lc, 'i');
                            else if (!key_local.compare("root") && strm_local.eof()  && !value_local.empty())
                                lc.root = value_local;
                            else if (!key_local.compare("upload_pass") && strm_local.eof() && !value_local.empty())
                                lc.upload_pass = value_local;
                            else if (!key_local.compare("auto_index") && strm_local.eof() && !value_local.empty())
                                lc.auto_index = value_local;
                            else if (!key_local.compare("return") && strm_local.eof() && !value_local.empty())
                                lc.redirection = value_local;
                            else
                            {
                                std::cerr << "You must Follow Our Rules in Location" << std::endl;
                                exit(1);
                            }

                        }
                        if (j != 0)
                        {
                            std::cerr << "You must Follow Our Rules" << std::endl;
                            exit(1);
                        }
                    }
                    else if (!line.compare("}"))
                    {
                        servers.push_back(sr);
                        sr.err_pages.clear();
                        sr.locations.clear();
                        i--;
                        break;
                    }
                    else if (!key.compare("host") && strm.eof() && !value.empty())
                        sr.host = value;
                    else if (!key.compare("port") && strm.eof() && !value.empty())
                        sr.port = value;
                    else if (!key.compare("max_client_body_size") && strm.eof() && !value.empty())
                        sr.mx_cl_bd_size = std::atoi(value.c_str());
                    else if (!key.compare("server_name"))
                        sr.server_name = value;
                    else if (!key.compare("error_page"))
                    {
                        if (strm.eof())
                            std::cerr << "You must Follow Our Rules" << std::endl;
                        strm >> key;
                        if (!strm.eof()) {
                            std::cerr << "You must Follow Our Rules" << std::endl;
                        }
                            sr.err_pages[value] = key;
                    }
                    else
                    {
                        std::cerr << "You must Follow Our Rules" << std::endl;
                        exit(1);
                    }
                }
            }
            if (i != 0)
            {
                std::cerr << "You must Follow Our Rules" << std::endl;
                exit(1);
            }
        }
    }
    else
    {
        std::cerr << "Error: can't open config file\n";
        exit(1);
    }
    return (0);
}