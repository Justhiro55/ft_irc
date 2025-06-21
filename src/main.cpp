#include "../includes/irc.hpp"
#include <signal.h>

// signal handling
IRCServer* g_server = NULL;

__attribute__((destructor)) static void destructor()
{
    system("leaks -q a.out");
}

void signal_handler(int signum) {
    std::cout << "\nShutting down server (signal: " << signum << ")..." << std::endl;
    if (g_server) {
        g_server->stop();
        delete g_server;
        g_server = NULL;
    }
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port number: " << port << std::endl;
        return 1;
    }

    std::string password = argv[2];
    if (password.empty()) {
        std::cerr << "Password cannot be empty" << std::endl;
        return 1;
    }

    signal(SIGINT, signal_handler);   // Ctrl+C
    signal(SIGTERM, signal_handler);  // Terminate signal

    try {
        // Create and start IRC server
        g_server = new IRCServer(port, password);
        std::cout << "Starting IRC server on port " << port << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;

        g_server->start();

    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    if (g_server) {
        delete g_server;
    }

    return 0;
}
