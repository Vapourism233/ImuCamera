#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <atomic>

#include "src/app/Application.h"
#include "src/utils/Logger.h"

// Global flag for graceful shutdown
std::atomic<bool> g_shutdown_requested{false};

/**
 * Signal handler for graceful shutdown
 */
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\n[SIGNAL] Shutdown requested..." << std::endl;
        g_shutdown_requested = true;
    }
}

/**
 * Main entry point
 */
int main(int argc, char* argv[]) {
    try {
        // Initialize logger
        auto& logger = utils::Logger::getInstance();
        logger.init("./logs/imu_camera.log", utils::Logger::Level::INFO);

        // Print banner
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║   IMU-Controlled PTZ Camera System    ║\n";
        std::cout << "║   Professional Implementation 2026    ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\n";

        logger.info("Application starting...");

        // Setup signal handlers for graceful shutdown
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);

        // Determine config file path
        std::string config_file = "config.ini";
        if (argc > 1) {
            config_file = argv[1];
            logger.infof("Using config file: %s (from command line)", config_file.c_str());
        } else {
            logger.infof("Using default config file: %s", config_file.c_str());
        }

        // Create and initialize application
        auto app = std::make_unique<app::Application>();

        if (!app->init(config_file)) {
            logger.error("Failed to initialize application");
            return 1;
        }

        // Main loop - check for shutdown signals
        std::thread app_thread([&app]() {
            app->run();
        });

        // Wait for shutdown signal
        while (!g_shutdown_requested && app->isRunning()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Request application shutdown
        app->requestShutdown();

        // Wait for app thread to finish
        if (app_thread.joinable()) {
            app_thread.join();
        }

        // Cleanup
        app->shutdown();
        app.reset();

        logger.info("Application shutdown complete");
        std::cout << "\nGoodbye!\n\n";

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}
