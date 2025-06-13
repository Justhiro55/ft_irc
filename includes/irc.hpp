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
#include "vector"

#include "Client.hpp"
#include "ServerData.hpp"
#include "Message.hpp"

class dupServer {
	void parsing(std::string &request);
};

// defines
#define MAX_PENDING 5
#define BUFFER_SIZE 1024
#define MESSAGE_MAX_LEN 512
#define MAX_CLIENTS 128

class AbstractCommand;

// IRC Server class
class IRCServer {
private:
    int server_fd;
    int port;
    std::string password;
    std::vector<struct pollfd> poll_fds;
    std::map<int, Client*> clients;
    ServerData* serverData;

public:
    IRCServer(int server_port, const std::string& server_password);
    ~IRCServer();

    void start();
    void stop();

    Client* getClient(int client_fd);
    ServerData* getServerData();
    const std::string& get_password() const;

private:
    int create_socket();
    void setup_server();
    void add_client(int client_fd, const std::string& ip, int port);
    void remove_client(int client_fd);
    void handle_new_connection();
    void handle_client_data(int client_fd);
    void handle_client_send(int client_fd);
    void parse_messages(int client_fd);
    void process_commands(int client_fd);
    void send_to_client(int client_fd, const std::string& message);
    void die_with_error(const char* msg, int fd);
    AbstractCommand* createCommand(const std::string& command);
};

#endif
