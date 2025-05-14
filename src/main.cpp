#include "../includes/irc.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);

    int server_fd = create_socket();

    // structure: local address
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // IPv4, internet address family
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // wildcard address
    addr.sin_port = htons(port); // port number

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        die_with_error("bind() failed", server_fd);

    if (listen(server_fd, MAX_PENDING) < 0)
        die_with_error("listen() failed", server_fd);

    std::cout << "Listening on port " << port << "..." << std::endl;

    for(;;)
    {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_sock < 0)
        {
            close(client_sock);
            die_with_error("accept() failed", server_fd);
        }

        std::cout << std::endl << "----------------------------------------" << std::endl;
        // std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
        // handle_tcp_client(client_sock);
        std::string body = handle_request(client_sock);

        std::cout << "Body: " << std::endl;
        std::cout << body << std::endl;

        close(client_sock);
    }

    close(server_fd);
    return 0;
}
