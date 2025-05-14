#include "../includes/irc.hpp"

void die_with_error(const char* msg, int fd)
{
    if (fd > 2)
        close(fd);
    perror(msg);
    exit(EXIT_FAILURE);
}

int create_socket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        die_with_error("socket() failed", sock);

    // set socket option to reuse address to prevent TIME_WAIT issues
    int yes = true;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes)) < 0)
        die_with_error("setsockopt() failed", sock);
    return sock;
}

std::string get_body(const std::string& request)
{
    size_t pos = request.find("\r\n\r\n");
    if (pos != std::string::npos)
    {
        return request.substr(pos + 4);
    }
    return "";
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
    std::cout << "Received: " << get_body(buffer) << std::endl;

    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_sock, response, strlen(response), 0);
}
