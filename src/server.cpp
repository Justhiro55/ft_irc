#include "../includes/irc.hpp"

// Constructor
IRCServer::IRCServer(int server_port, const std::string& server_password)
    : server_fd(-1), port(server_port), password(server_password) {
    serverData = new ServerData();
    serverData->setPassword(server_password);
    setup_server();
}

// Destructor
IRCServer::~IRCServer() {
    stop();
    delete serverData;
}

void IRCServer::die_with_error(const char* msg, int fd)
{
    if (fd > 2)
        close(fd);
    perror(msg);
    exit(EXIT_FAILURE);
}

int IRCServer::create_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        die_with_error("socket() failed", sock);

    int yes = true;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes)) < 0)
        die_with_error("setsockopt() failed", sock);

    return sock;
}

void IRCServer::setup_server() {
    server_fd = create_socket();

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

    // non-blocking mode
    int flags = fcntl(server_fd, F_GETFL, 0);
    if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) < 0)
        die_with_error("fcntl() failed", server_fd);

    // Add server socket to poll array
    struct pollfd server_poll_fd;
    server_poll_fd.fd = server_fd;
    server_poll_fd.events = POLLIN; // Monitor for incoming connections
    server_poll_fd.revents = 0;
    poll_fds.push_back(server_poll_fd);

    std::cout << "IRC Server listening on port " << port << "..." << std::endl;
}

void IRCServer::start() {
    while (true) {
        // timeout of 1 second
        int poll_result = poll(&poll_fds[0], poll_fds.size(), 1000);

        if (poll_result < 0) {
            perror("poll() failed");
            break;
        }

        if (poll_result == 0) {
            // Timeout
            continue;
        }

        // Check each file descriptor
        for (size_t i = 0; i < poll_fds.size(); ++i) {
            // 受信データの処理
            if (poll_fds[i].revents & POLLIN) {
                if (poll_fds[i].fd == server_fd) {
                    // New connection
                    handle_new_connection();
                } else {
                    // Data available
                    handle_client_data(poll_fds[i].fd);
                }
            }

            // 送信データの処理
            if (poll_fds[i].revents & POLLOUT) {
                if (poll_fds[i].fd != server_fd) {
                    handle_client_send(poll_fds[i].fd);
                }
            }

            // Handle client disconnection or errors
            if (poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
                if (poll_fds[i].fd != server_fd) {
                    std::cout << "Client disconnected (fd: " << poll_fds[i].fd << ")" << std::endl;
                    remove_client(poll_fds[i].fd);
                    i--; // Adjust index after removal
                }
            }
        }
    }
}

void IRCServer::stop() {
    // Close all client connections
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        close(it->second->getClientFd());
        delete it->second;
    }
    clients.clear();
    poll_fds.clear();

    // Close server socket
    if (server_fd >= 0) {
        close(server_fd);
        server_fd = -1;
    }

    std::cout << "IRC Server stopped." << std::endl;
}

void IRCServer::handle_new_connection() {
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("accept() failed");
        }
        return;
    }

    // non-blocking mode
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl() failed for client socket");
        close(client_fd);
        return;
    }

    // Get client IP and port
    std::string client_ip = inet_ntoa(client_addr.sin_addr);
    int client_port = ntohs(client_addr.sin_port);

    add_client(client_fd, client_ip, client_port);

    std::cout << "New connection from " << client_ip << ":" << client_port
              << " (fd: " << client_fd << ")" << std::endl;

    send_to_client(client_fd, "ft_irc server ready\r\n");
}

void IRCServer::add_client(int client_fd, const std::string& ip, int port) {
    // Create new client
    Client* new_client = new Client();
    new_client->setClientFd(client_fd);
    new_client->setIp(ip);
    new_client->setPort(port);

    clients[client_fd] = new_client;
    serverData->setClient(new_client);

    // Add to poll array
    struct pollfd client_poll_fd;
    client_poll_fd.fd = client_fd;
    client_poll_fd.events = POLLIN; // Monitor for incoming data
    client_poll_fd.revents = 0;
    poll_fds.push_back(client_poll_fd);
}

void IRCServer::remove_client(int client_fd) {
    // Remove from clients map
    std::map<int, Client*>::iterator it = clients.find(client_fd);
    if (it == clients.end()) {
        return;
    }

    Client* client = it->second;

    // Remove ServerData
    serverData->removeClient(client);

    // Remove clients map
    delete client;
    clients.erase(it);

    // Remove poll_fds
    for (size_t i = 0; i < poll_fds.size(); ++i) {
        if (poll_fds[i].fd == client_fd) {
            poll_fds.erase(poll_fds.begin() + i);
            break;
        }
    }

    close(client_fd);
}

void IRCServer::handle_client_data(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("recv() failed");
            remove_client(client_fd);
        }
        return;
    }

    if (bytes_received == 0) {
        // Client disconnected
        std::cout << "Client disconnected (fd: " << client_fd << ")" << std::endl;
        remove_client(client_fd);
        return;
    }

    buffer[bytes_received] = '\0';
    std::string data(buffer);

    // Find client
    std::map<int, Client*>::iterator it = clients.find(client_fd);
    if (it == clients.end()) {
        return; // Client not found
    }

    Client* client = it->second;

    client->getBuffer() += data;

    std::cout << "Received from " << client->getIp() << ":" << client->getPort() << ": " << data;

    // 受信したデータから"\r\n"で終わるメッセージを処理する
    parse_messages(client_fd);
}

void IRCServer::parse_messages(int client_fd) {
    std::map<int, Client*>::iterator it = clients.find(client_fd);
    if (it == clients.end()) {
        return; // Client not found
    }

    Client* client = it->second;
    std::string& buffer = client->getBuffer();

    // Process complete messages in buffer
    size_t pos = 0;
    while ((pos = buffer.find("\r\n")) != std::string::npos) {
        size_t message_length = pos;
        if (message_length > MESSAGE_MAX_LEN) {
            std::cout << "Message too long (" << message_length
                      << " bytes) from client " << client_fd << std::endl;
            send_error_message(client_fd, "ERROR :Message too long\r\n");
            buffer.erase(0, pos + 2);
            continue;
        }

        // Extract complete message
        std::string message = buffer.substr(0, pos);

        // バッファから処理済みメッセージ削除
        buffer.erase(0, pos + 2);

        // Skip empty messages
        if (message.empty()) {
            continue;
        }

        std::cout << "Complete message from " << client->getIp() << ":" << client->getPort()
                  << ": '" << message << "'" << std::endl;

        // メッセージのparseと受信キューへの追加
        Message parsed_message = tokenizeMessage(message);
        std::cout << "Parsed Message:" << std::endl;
        std::cout << "  Prefix: " << parsed_message.prefix << std::endl;
        std::cout << "  Command: " << parsed_message.command << std::endl;
        std::cout << "  Params:";
        for (size_t i = 0; i < parsed_message.params.size(); ++i) {
            std::cout << " [" << parsed_message.params[i] << "]";
        }
        std::cout << std::endl;

        client->pushMessageToRecvQueue(parsed_message);

        std::cout << "Message added to recvQueue - Command: " << parsed_message.command
                  << ", Params: " << parsed_message.params.size() << std::endl;

        std::string echo_message = "ECHO :" + message + "\r\n";
        send_to_client(client_fd, echo_message);
    }

    // Check buffer size to prevent memory issues
    if (buffer.length() > BUFFER_SIZE * 2) {
        std::cout << "Buffer overflow protection: clearing buffer for client " << client_fd << std::endl;
        buffer.clear();
        send_error_message(client_fd, "ERROR :Message too long\r\n");
    }
}

void IRCServer::handle_client_send(int client_fd) {
    std::map<int, Client*>::iterator it = clients.find(client_fd);
    if (it == clients.end()) {
        return; // Client not found
    }

    Client* client = it->second;
    std::queue<std::string>& sendQueue = client->getSendQueue();

    // 送信キューにあるメッセージを処理
    while (!sendQueue.empty()) {
        const std::string& message = sendQueue.front();

        ssize_t bytes_sent = send(client_fd, message.c_str(), message.length(), 0);

        if (bytes_sent < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            } else {
                perror("send() failed");
                remove_client(client_fd);
                return;
            }
        } else if (bytes_sent < (ssize_t)message.length()) {
            // 一部送信 - 残りのデータを更新
            std::string remaining = message.substr(bytes_sent);
            sendQueue.pop();
            sendQueue.push(remaining);
            break;
        } else {
            // 送信完了 - メッセージをキューから削除
            sendQueue.pop();
        }
    }

    // send queueが空になったら，POLLOUTを外す
    if (sendQueue.empty()) {
        for (size_t i = 0; i < poll_fds.size(); ++i) {
            if (poll_fds[i].fd == client_fd) {
                poll_fds[i].events &= ~POLLOUT;
                break;
            }
        }
    }
}

void IRCServer::send_to_client(int client_fd, const std::string& message) {
    std::map<int, Client*>::iterator it = clients.find(client_fd);
    if (it == clients.end()) {
        return; // Client not found
    }

    Client* client = it->second;
    client->pushToSendQueue(message);

    // 送信可能になったらpoll()で監視できるようにPOLLOUTを設定
    for (size_t i = 0; i < poll_fds.size(); ++i) {
        if (poll_fds[i].fd == client_fd) {
            poll_fds[i].events |= POLLOUT;
            break;
        }
    }
}

void IRCServer::send_error_message(int client_fd, const std::string& message) {
    ssize_t bytes_sent = send(client_fd, message.c_str(), message.length(), 0);
    if (bytes_sent < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("send_error_message() failed");
        }
    }
}

Client* IRCServer::getClient(int client_fd) {
    std::map<int, Client*>::iterator it = clients.find(client_fd);
    if (it != clients.end()) {
        return it->second;
    }
    return NULL;
}

ServerData* IRCServer::getServerData() {
    return serverData;
}

const std::string& IRCServer::get_password() const {
    return password;
}
