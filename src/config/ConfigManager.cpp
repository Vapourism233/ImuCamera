#include "ConfigManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace config {

bool ConfigManager::loadFromFile(const std::string& config_file) {
    try {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            logger_.warningf("Config file not found: %s, using defaults", config_file.c_str());
            loaded_ = true;  // Still consider it loaded with defaults
            return true;
        }

        logger_.infof("Loading configuration from: %s", config_file.c_str());

        // Extract base directory from config file path
        std::string config_dir = config_file;
        size_t last_slash = config_dir.find_last_of("/\\");
        if (last_slash != std::string::npos) {
            config_dir = config_dir.substr(0, last_slash + 1);
        } else {
            config_dir = "./";
        }
        base_directory_ = config_dir;
        config_directory_ = config_dir;

        std::string line;
        std::string current_section;

        while (std::getline(file, line)) {
            // Trim whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') {
                continue;
            }

            // Check for section headers [section]
            if (line[0] == '[' && line[line.length() - 1] == ']') {
                current_section = line.substr(1, line.length() - 2);
                logger_.debugf("Config section: [%s]", current_section.c_str());
                continue;
            }

            // Parse key=value pairs
            size_t eq_pos = line.find('=');
            if (eq_pos == std::string::npos) {
                continue;
            }

            std::string key = line.substr(0, eq_pos);
            std::string value = line.substr(eq_pos + 1);

            // Trim whitespace from key and value
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Remove quotes if present
            if (value.length() >= 2 && value[0] == '"' && value[value.length() - 1] == '"') {
                value = value.substr(1, value.length() - 2);
            }

            logger_.debugf("Config: [%s] %s = %s", current_section.c_str(), key.c_str(), value.c_str());

            // IMU section
            if (current_section == "imu") {
                if (key == "device") {
                    imu_device_ = value;
                } else if (key == "baudrate") {
                    imu_baudrate_ = std::stoul(value);
                }
            }
            // Camera section
            else if (current_section == "camera") {
                if (key == "ip") {
                    camera_ip_ = value;
                } else if (key == "port") {
                    camera_port_ = static_cast<uint16_t>(std::stoul(value));
                } else if (key == "username") {
                    camera_username_ = value;
                } else if (key == "password") {
                    camera_password_ = value;
                } else if (key == "channel") {
                    camera_channel_ = std::stoul(value);
                }
            }
            // Logging section
            else if (current_section == "logging") {
                if (key == "file") {
                    log_file_ = value;
                } else if (key == "level") {
                    // Normalize level string to uppercase
                    std::transform(value.begin(), value.end(), value.begin(), ::toupper);
                    if (value == "DEBUG") {
                        log_level_ = utils::Logger::Level::DEBUG;
                    } else if (value == "INFO") {
                        log_level_ = utils::Logger::Level::INFO;
                    } else if (value == "WARNING") {
                        log_level_ = utils::Logger::Level::WARNING;
                    } else if (value == "ERROR") {
                        log_level_ = utils::Logger::Level::ERROR;
                    } else if (value == "CRITICAL") {
                        log_level_ = utils::Logger::Level::CRITICAL;
                    }
                }
            }
            // Vision section
            else if (current_section == "vision") {
                if (key == "save_regions") {
                    vision_save_regions_ = (value == "true" || value == "1" || value == "yes");
                } else if (key == "show_steps") {
                    vision_show_steps_ = (value == "true" || value == "1" || value == "yes");
                }
            }
            // Paths section
            else if (current_section == "paths") {
                if (key == "models") {
                    models_directory_ = value;
                } else if (key == "output") {
                    output_directory_ = value;
                } else if (key == "logs") {
                    log_directory_ = value;
                }
            }
        }

        loaded_ = true;
        logger_.info("Configuration loaded successfully");
        return true;

    } catch (const std::exception& e) {
        logger_.errorf("Exception while loading config: %s", e.what());
        loaded_ = false;
        return false;
    }
}

hardware::HardwareManager::Config ConfigManager::getHardwareConfig() const {
    hardware::HardwareManager::Config config;
    
    config.imu_device = imu_device_;
    config.imu_baudrate = imu_baudrate_;
    
    config.camera_ip = camera_ip_;
    config.camera_port = camera_port_;
    config.camera_username = camera_username_;
    config.camera_password = camera_password_;
    config.camera_channel = camera_channel_;
    
    config.log_file = log_file_;
    config.log_level = log_level_;
    
    return config;
}

std::string ConfigManager::getModelPath(const std::string& model_name) const {
    // If model_name is already an absolute path, return as is
    if (model_name.length() > 0 && model_name[0] == '/') {
        return model_name;
    }
    
    // Otherwise, construct path relative to models directory
    return models_directory_ + "/" + model_name;
}

}  // namespace config
