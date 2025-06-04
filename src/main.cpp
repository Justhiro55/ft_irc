#include "../includes/irc.hpp"

// クライアント情報を表示する関数
void print_client_info(const std::vector<std::string>& clients) {
    std::cout << "\n=== Connected Clients ===" << std::endl;
    for (size_t i = 0; i < clients.size(); ++i) {
        std::cout << i + 1 << ". " << clients[i] << std::endl;
    }
    std::cout << "======================\n" << std::endl;
}

// 1クライアントとの通信を処理する関数
void handle_single_client(int client_sock, const std::string& client_info) {
    char buffer[1024];
    std::string welcome_msg = "ft_irc server ready\r\n";

    if (send(client_sock, welcome_msg.c_str(), welcome_msg.length(), 0) < 0) {
        std::cerr << "send failed" << std::endl;
        return;
    }

    while (true) {
        ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(10000);
                continue;
            }
            std::cerr << "recv error: " << strerror(errno) << std::endl;
            break;
        }
        else if (bytes_received == 0) {
            std::cout << "client disconnected" << std::endl;
            break;
        }

        buffer[bytes_received] = '\0';
        std::string data(buffer);

        std::cout << "recv: " << data;

        if (data.find("QUIT") != std::string::npos) {
            std::string quit_msg = "Closing connection\r\n";
            send(client_sock, quit_msg.c_str(), quit_msg.length(), 0);
            break;
        }

        // simple echo for now
        std::string response = ":" + client_info + " PRIVMSG echo :" + data;
        if (response.back() != '\n') {
            response += "\r\n";
        }

        if (send(client_sock, response.c_str(), response.length(), 0) < 0) {
            std::cerr << "send failed" << std::endl;
            break;
        }
    }
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
        die_with_error("bind() failed", server_fd);

    if (listen(server_fd, MAX_PENDING) < 0)
        die_with_error("listen() failed", server_fd);

    // ノンブロッキングモードに設定
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    std::cout << "Listening on port " << port << "..." << std::endl;

    // クライアント情報を保持するベクター
    std::vector<std::string> client_history;

    for (;;)
    {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_sock < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) // Blocking mode
            {
                usleep(10000);
                continue;
            }
            std::cerr << "accept() failed: " << strerror(errno) << std::endl;
            break;
        }

        // 新しいクライアントをノンブロッキングモードに設定
        flags = fcntl(client_sock, F_GETFL, 0);
        fcntl(client_sock, F_SETFL, flags | O_NONBLOCK);

        // クライアント情報を記録
        std::ostringstream oss;
        oss << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);
        std::string client_info = oss.str();
        client_history.push_back(client_info);

        std::cout << "new connection from " << client_info << std::endl;

        handle_single_client(client_sock, client_info);

        close(client_sock);
        std::cout << "connection closed" << std::endl;
    }

    close(server_fd);
    return 0;
}
