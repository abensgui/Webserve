#include "configuration_file/configuration.hpp"
#include "Server/socketServer.hpp"

int main(int ac , char **av)
{
    if (ac != 2)
        exit (1);
    signal(SIGPIPE, SIG_IGN);
    confi cfg;
    SocketServer skt;
    cfg.setup_configuration(av[1]);
    skt.run_server(cfg.servers);
}
