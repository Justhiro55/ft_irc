#include "../includes/irc.hpp"

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
