#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>
#include "../hardware/HardwareManager.h"
#include "../config/ConfigManager.h"
#include "../utils/Logger.h"

namespace app {

/**
 * @brief Main application class
 * Orchestrates all system components: hardware, vision processing, etc.
 */
class Application {
public:
    /**
     * Constructor
     */
    Application();

    /**
     * Destructor
     */
    ~Application();

    /**
     * Initialize the application
     * @param config_file Path to configuration file
     * @return true if successful, false otherwise
     */
    bool init(const std::string& config_file);

    /**
     * Run the main application loop
     * @return Exit code (0 = success)
     */
    int run();

    /**
     * Shutdown the application
     * @return true if successful, false otherwise
     */
    bool shutdown();

    /**
     * Check if application is running
     * @return true if running, false otherwise
     */
    bool isRunning() const { return running_; }

    /**
     * Request application shutdown
     */
    void requestShutdown() { running_ = false; }

private:
    /**
     * Main processing loop
     */
    void mainLoop();

    /**
     * Handle IMU data and update camera position
     */
    void handleIMUData();

    /**
     * Capture and process image from camera
     */
    void processImage();

    // Components
    hardware::HardwareManager& hardware_manager_;
    config::ConfigManager& config_manager_;
    utils::Logger& logger_;

    // State
    bool running_{false};
    bool initialized_{false};
};

}  // namespace app

#endif  // APPLICATION_H
