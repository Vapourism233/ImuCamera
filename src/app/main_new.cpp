#include "Application.h"
#include "../utils/Logger.h"
#include <iostream>
#include <csignal>
#include <atomic>

// Global flag for graceful shutdown
std::atomic<bool> shutdown_requested(false);

// Signal handler for Ctrl+C (SIGINT) and termination (SIGTERM)
void signalHandler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        std::cout << "\n\nShutdown signal received. Cleaning up...\n";
        shutdown_requested = true;
    }
}

int main(int argc, char* argv[]) {
    // Setup signal handlers for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    try {
        // Determine config file path
        std::string config_file = "config.ini";
        if (argc > 1) {
            config_file = argv[1];
        }
        
        // Initialize application
        app::Application app;
        
        if (!app.init(config_file)) {
            std::cerr << "Failed to initialize application\n";
            return 1;
        }
        
        // Run application
        if (!app.run()) {
            std::cerr << "Application terminated with error\n";
            return 1;
        }
        
        // Graceful shutdown
        app.shutdown();
        
        std::cout << "Application terminated successfully\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}
