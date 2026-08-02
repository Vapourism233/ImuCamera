#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <memory>
#include "../utils/Logger.h"
#include "../hardware/HardwareManager.h"

namespace config {

/**
 * @brief Configuration Manager - Load and manage application settings
 * 
 * Supports YAML configuration files with fallback defaults
 * 
 * Example config.yaml:
 * @code
 * # IMU Configuration
 * imu:
 *   device: "/dev/ttyUSB0"
 *   baudrate: 115200
 * 
 * # Camera Configuration
 * camera:
 *   ip: "192.168.1.8"
 *   port: 8000
 *   username: "admin"
 *   password: "a1234567"
 *   channel: 1
 * 
 * # Logging Configuration
 * logging:
 *   file: "./logs/imu_camera.log"
 *   level: "INFO"  # DEBUG, INFO, WARNING, ERROR, CRITICAL
 * 
 * # Vision Configuration
 * vision:
 *   save_regions: false
 *   show_steps: false
 * @endcode
 */
class ConfigManager {
public:
    /**
     * Get singleton instance
     */
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    // Delete copy/move operations
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    /**
     * Load configuration from file
     * @param config_file Path to configuration file
     * @return true if successful, false otherwise
     */
    bool loadFromFile(const std::string& config_file);

    /**
     * Load default configuration
     * @return Configuration with default values
     */
    hardware::HardwareManager::Config getHardwareConfig() const;

    /**
     * Get application base directory (for data files, models, etc.)
     * @return Base directory path
     */
    std::string getBaseDirectory() const { return base_directory_; }

    /**
     * Get model file path
     * @param model_name Name of the model (e.g., "SVM.xml", "OCR.xml")
     * @return Full path to model file
     */
    std::string getModelPath(const std::string& model_name) const;

    /**
     * Get output directory for results
     * @return Output directory path
     */
    std::string getOutputDirectory() const { return output_directory_; }

    /**
     * Check if configuration is loaded
     * @return true if loaded successfully, false otherwise
     */
    bool isLoaded() const { return loaded_; }

private:
    ConfigManager() = default;

    // Configuration state
    bool loaded_{false};
    
    // Paths
    std::string base_directory_{"./"};
    std::string config_directory_{"./"};
    std::string models_directory_{"./models"};
    std::string output_directory_{"./output"};
    std::string log_directory_{"./logs"};
    
    // Hardware settings
    std::string imu_device_{"/dev/ttyUSB0"};
    uint32_t imu_baudrate_{115200};
    
    std::string camera_ip_{"192.168.1.8"};
    uint16_t camera_port_{8000};
    std::string camera_username_{"admin"};
    std::string camera_password_{"a1234567"};
    uint32_t camera_channel_{1};
    
    // Logging settings
    std::string log_file_{"./logs/imu_camera.log"};
    utils::Logger::Level log_level_{utils::Logger::Level::INFO};
    
    // Vision settings
    bool vision_save_regions_{false};
    bool vision_show_steps_{false};
    
    // Logging
    utils::Logger& logger_ = utils::Logger::getInstance();
};

}  // namespace config

#endif  // CONFIG_MANAGER_H
