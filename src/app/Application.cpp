#include "Application.h"
#include <chrono>
#include <thread>

namespace app {

Application::Application()
    : hardware_manager_(hardware::HardwareManager::getInstance()),
      config_manager_(config::ConfigManager::getInstance()),
      logger_(utils::Logger::getInstance()) {
}

Application::~Application() {
    shutdown();
}

bool Application::init(const std::string& config_file) {
    if (initialized_) {
        logger_.warning("Application is already initialized");
        return true;
    }

    logger_.info("========== Application Initialization Starting ==========");

    // Load configuration
    if (!config_manager_.loadFromFile(config_file)) {
        logger_.error("Failed to load configuration");
        return false;
    }

    // Initialize hardware manager
    hardware::HardwareManager::Config hw_config = config_manager_.getHardwareConfig();
    if (!hardware_manager_.init(hw_config)) {
        logger_.error("Failed to initialize hardware manager");
        return false;
    }

    // Start IMU reader
    if (!hardware_manager_.startIMU()) {
        logger_.warning("Failed to start IMU reader, continuing without IMU");
    }

    // Connect to camera
    if (!hardware_manager_.connectCamera()) {
        logger_.warning("Failed to connect to camera, continuing without camera");
    }

    initialized_ = true;
    logger_.info("========== Application Initialization Complete ==========");
    return true;
}

int Application::run() {
    if (!initialized_) {
        logger_.error("Application not initialized");
        return 1;
    }

    logger_.info("========== Application Started ==========");
    running_ = true;

    mainLoop();

    logger_.info("========== Application Stopped ==========");
    return 0;
}

bool Application::shutdown() {
    if (!initialized_) {
        return true;
    }

    logger_.info("========== Application Shutdown Starting ==========");

    running_ = false;

    // Shutdown hardware
    if (!hardware_manager_.shutdown()) {
        logger_.error("Error during hardware shutdown");
    }

    initialized_ = false;
    logger_.info("========== Application Shutdown Complete ==========");
    return true;
}

void Application::mainLoop() {
    const auto frame_duration = std::chrono::milliseconds(33);  // ~30 FPS
    const size_t print_interval = 30;  // Print status every 30 frames
    size_t frame_count = 0;

    while (running_) {
        auto frame_start = std::chrono::high_resolution_clock::now();

        // Process IMU data
        handleIMUData();

        // Process image
        processImage();

        // Frame timing
        auto frame_end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            frame_end - frame_start);

        // Maintain target frame rate
        if (elapsed < frame_duration) {
            std::this_thread::sleep_for(frame_duration - elapsed);
        }

        frame_count++;
        if (frame_count % print_interval == 0) {
            logger_.infof("Frame %zu: IMU running=%s, Camera connected=%s",
                         frame_count,
                         hardware_manager_.isIMURunning() ? "yes" : "no",
                         hardware_manager_.isCameraConnected() ? "yes" : "no");
        }
    }
}

void Application::handleIMUData() {
    hardware::IMUData imu_data;

    if (hardware_manager_.getLatestIMUData(imu_data)) {
        // Update camera position based on IMU data
        if (!hardware_manager_.updateCameraPosition(imu_data)) {
            logger_.debug("Failed to update camera position");
        }
    }
}

void Application::processImage() {
    if (!hardware_manager_.isCameraConnected()) {
        return;
    }

    // TODO: Implement image capture and processing
    // - Capture frame from camera
    // - Detect license plates
    // - Recognize plate characters
    // - Save results
    
    // Placeholder
}

}  // namespace app
