#ifndef IRC_HPP
#define IRC_HPP

// includes
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <poll.h>
#include <errno.h>
#include <map>

// defines
#define MAX_PENDING 5
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 128


struct Client {
    int fd;
    std::string ip;
    int port;
    std::string buffer;

    Client(int socket_fd, const std::string& client_ip, int client_port)
        : fd(socket_fd), ip(client_ip), port(client_port) {}
};

// IRC Server class
class IRCServer {
private:
    int server_fd;
    int port;
    std::vector<struct pollfd> poll_fds;
    std::map<int, Client*> clients;

public:
    IRCServer(int server_port);
    ~IRCServer();

    void start();
    void stop();

private:
    int create_socket();
    void setup_server();
    void add_client(int client_fd, const std::string& ip, int port);
    void remove_client(int client_fd);
    void handle_new_connection();
    void handle_client_data(int client_fd);
    void send_to_client(int client_fd, const std::string& message);
    void die_with_error(const char* msg, int fd);
};

#endif
