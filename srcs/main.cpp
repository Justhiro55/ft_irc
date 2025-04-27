#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "socket() failed" << std::endl;
        return 1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "bind() failed" << std::endl;
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0)
    {
        std::cerr << "listen() failed" << std::endl;
        close(server_fd);
        return 1;
    }

    std::cout << "Listening on port " << port << "..." << std::endl;

    pause();

    close(server_fd);
    return 0;
}
