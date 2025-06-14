#include "../includes/irc.hpp"
#include "../includes/Command.hpp"

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
                    std::cout << "POLLOUT event for client " << poll_fds[i].fd << std::endl;
                    handle_client_send(poll_fds[i].fd);
                }
            }

            // Handle client disconnection or errors
            if (poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
                if (poll_fds[i].fd != server_fd) {
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
    // Check: max_clients limit
    if (clients.size() >= MAX_CLIENTS) {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);

        if (client_fd >= 0) {
            std::string client_ip = inet_ntoa(client_addr.sin_addr);
            (void)ntohs(client_addr.sin_port);

            std::cout << "Connection limit reached." << std::endl;

            const char* error_msg = "ERROR :Server full\r\n";
            send(client_fd, error_msg, strlen(error_msg), 0);
            close(client_fd);
        }
        return;
    }

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
              << " (fd: " << client_fd << ") - Total: "
              << clients.size() << "/" << MAX_CLIENTS << std::endl;
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
    Client* client = findClient(client_fd);
    if (!client) {
        return; // Client not found
    }

    client->getBuffer() += data;

    std::cout << "Received from " << client->getIp() << ":" << client->getPort() << ": " << data;

    // 受信したデータから"\r\n"で終わるメッセージを処理する
    parse_messages(client_fd);
}

void IRCServer::parse_messages(int client_fd) {
    Client* client = findClient(client_fd);
    if (!client) {
        return; // Client not found
    }
    std::string& buffer = client->getBuffer();

    // Process complete messages in buffer
    size_t pos = findLineEnd(buffer);
    
    while (pos != std::string::npos) {
        size_t message_length = pos;
        if (message_length > MESSAGE_MAX_LEN) {
            std::cout << "Message too long (" << message_length
                      << " bytes) from client " << client_fd << std::endl;
            send_to_client(client_fd, "ERROR :Message too long\r\n");
            buffer.erase(0, pos + 2);
            continue;
        }

        // Extract complete message
        std::string message = buffer.substr(0, pos);

        // バッファから処理済みメッセージ削除
        bool is_crlf = (pos < buffer.length() - 1 && buffer[pos] == '\r' && buffer[pos + 1] == '\n');
        buffer.erase(0, pos + (is_crlf ? 2 : 1));

        // Skip empty messages
        if (message.empty()) {
            std::cout << "Skipping empty message" << std::endl;
            // Find next line ending
            pos = findLineEnd(buffer);
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
                  
        // Find next line ending
        pos = findLineEnd(buffer);
    }
    
    // Process all commands in the receive queue
    process_commands(client_fd);

    // Check buffer size to prevent memory issues
    if (buffer.length() > BUFFER_SIZE * 2) {
        std::cout << "Buffer overflow protection: clearing buffer for client " << client_fd << std::endl;
        buffer.clear();
        send_to_client(client_fd, "ERROR :Message too long\r\n");
    }
}

void IRCServer::handle_client_send(int client_fd) {
    Client* client = findClient(client_fd);
    if (!client) {
        return; // Client not found
    }
    std::queue<std::string>& sendQueue = client->getSendQueue();

    std::cout << "handle_client_send: " << sendQueue.size() << " messages in queue" << std::endl;

    // 送信キューにあるメッセージを処理
    while (!sendQueue.empty()) {
        const std::string& message = sendQueue.front();
        
        std::cout << "Sending: " << message << std::endl;

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
    Client* client = findClient(client_fd);
    if (!client) {
        return; // Client not found
    }
    client->pushToSendQueue(message);

    // 送信可能になったらpoll()で監視できるようにPOLLOUTを設定
    for (size_t i = 0; i < poll_fds.size(); ++i) {
        if (poll_fds[i].fd == client_fd) {
            poll_fds[i].events |= POLLOUT;
            break;
        }
    }
}

Client* IRCServer::getClient(int client_fd) {
    return findClient(client_fd);
}

ServerData* IRCServer::getServerData() {
    return serverData;
}

const std::string& IRCServer::get_password() const {
    return password;
}

void IRCServer::process_commands(int client_fd) {
    Client* client = findClient(client_fd);
    if (!client) {
        return;
    }
    std::queue<Message>& recvQueue = client->getRecvQueue();

    while (!recvQueue.empty()) {
        Message message = recvQueue.front();
        recvQueue.pop();

        std::cout << "Processing command: " << message.command << std::endl;

        AbstractCommand* command = createCommand(message.command);
        if (command) {
            command->setMessage(message);
            command->setServerData(serverData);
            command->setExecuter(client);
            command->executeCmd();
            delete command;
            
            // Check if there are messages to send
            if (!client->getSendQueue().empty()) {
                std::cout << "Messages in send queue: " << client->getSendQueue().size() << std::endl;
                // Force send immediately
                handle_client_send(client_fd);
            }
        } else {
            std::cout << "Unknown command: " << message.command << std::endl;
        }
    }
}

AbstractCommand* IRCServer::createCommand(const std::string& command) {
    if (command == "PASS") {
        return new Pass();
    } else if (command == "NICK") {
        return new Nick();
    } else if (command == "USER") {
        return new User();
    } else if (command == "JOIN") {
        return new Join();
    } else if (command == "INVITE") {
        return new Invite();
    } else if (command == "KICK") {
        return new Kick();
    }
    return NULL;
}

// Helper functions
void IRCServer::die_with_error(const char* msg, int fd)
{
    if (fd > 2)
        close(fd);
    perror(msg);
    exit(EXIT_FAILURE);
}

Client* IRCServer::findClient(int client_fd) {
    std::map<int, Client*>::iterator it = clients.find(client_fd);
    if (it != clients.end()) {
        return it->second;
    }
    return NULL;
}

size_t IRCServer::findLineEnd(const std::string& buffer) {
    size_t crlf_pos = buffer.find("\r\n");
    size_t lf_pos = buffer.find("\n");
    
    if (crlf_pos != std::string::npos && (lf_pos == std::string::npos || crlf_pos <= lf_pos)) {
        return crlf_pos;
    } else if (lf_pos != std::string::npos) {
        return lf_pos;
    }
    return std::string::npos;
}

