#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
# include <stdio.h>
# include <string.h>

# include <sys/types.h>
# include <netdb.h>
# include <arpa/inet.h>

#define MAX_PENDING 5

void die_with_error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket() failed" << std::endl;
        return -1;
    }
    return sockfd;
}

void handle_tcp_client(int client_sock)
{
    char buffer[1024];
    ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0)
    {
        std::cerr << "recv() failed" << std::endl;
        return;
    }
    buffer[bytes_received] = '\0';
    std::cout << "Received: " << buffer << std::endl;

    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_sock, response, strlen(response), 0);
}

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
    {
        close(server_fd);
        die_with_error("bind() failed");
    }

    if (listen(server_fd, MAX_PENDING) < 0)
    {
        close(server_fd);
        die_with_error("listen() failed");
    }

    std::cout << "Listening on port " << port << "..." << std::endl;

    for(;;)
    {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_sock < 0)
        {
            close(server_fd);
            die_with_error("accept() failed");
        }

        std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

        handle_tcp_client(client_sock);

        close(client_sock);
    }
    // pause();

    close(server_fd);
    return 0;
}
