#ifndef SERVER_HPP
#define SERVER_HPP

// includes
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

#include "Client.hpp"
#include "Message.hpp"
#include "vector"


class dupServer {
	void parsing(std::string &request);
};




// defines
#define MAX_PENDING 5

void die_with_error(const char* msg, int fd);
int create_socket();
void handle_tcp_client(int client_sock);
std::string handle_request(int client_sock);

#endif
