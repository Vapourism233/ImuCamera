#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include <memory>
#include <string>
#include <cstdint>
#include "../utils/Logger.h"
#include "IMUReader.h"
#include "IMUData.h"
#include "CameraControl.h"

namespace hardware {

/**
 * @brief Hardware Manager - Central coordinator for all hardware components
 * 
 * Manages:
 * - IMU reader (serial communication)
 * - Camera controller (PTZ operations)
 * - Synchronization between components
 * 
 * Thread-safe singleton pattern
 * 
 * Example usage:
 * @code
 * auto& hwm = HardwareManager::getInstance();
 * if (hwm.init(config)) {
 *     hwm.startIMU();
 *     hwm.connectCamera();
 *     
 *     IMUData imu_data;
 *     if (hwm.getLatestIMUData(imu_data)) {
 *         // Update camera position based on IMU
 *         hwm.updateCameraPosition(imu_data);
 *     }
 *     
 *     hwm.shutdown();
 * }
 * @endcode
 */
class HardwareManager {
public:
    /**
     * Configuration structure for hardware
     */
    struct Config {
        // IMU configuration
        std::string imu_device;
        uint32_t imu_baudrate = 115200;
        
        // Camera configuration
        std::string camera_ip;
        uint16_t camera_port = 8000;
        std::string camera_username;
        std::string camera_password;
        uint32_t camera_channel = 1;
        
        // Logging
        std::string log_file;
        utils::Logger::Level log_level = utils::Logger::Level::INFO;
    };

    /**
     * Get singleton instance
     */
    static HardwareManager& getInstance() {
        static HardwareManager instance;
        return instance;
    }

    // Delete copy/move operations
    HardwareManager(const HardwareManager&) = delete;
    HardwareManager& operator=(const HardwareManager&) = delete;
    HardwareManager(HardwareManager&&) = delete;
    HardwareManager& operator=(HardwareManager&&) = delete;

    /**
     * Initialize hardware manager with configuration
     * @param config Hardware configuration
     * @return true if successful, false otherwise
     */
    bool init(const Config& config);

    /**
     * Shutdown all hardware components
     * @return true if successful, false otherwise
     */
    bool shutdown();

    /**
     * Start IMU reader
     * @return true if successful, false otherwise
     */
    bool startIMU();

    /**
     * Stop IMU reader
     * @return true if successful, false otherwise
     */
    bool stopIMU();

    /**
     * Connect to camera
     * @return true if successful, false otherwise
     */
    bool connectCamera();

    /**
     * Disconnect from camera
     * @return true if successful, false otherwise
     */
    bool disconnectCamera();

    /**
     * Get the latest IMU data (non-blocking)
     * @param data Reference to store IMU data
     * @return true if data was available, false if queue is empty
     */
    bool getLatestIMUData(IMUData& data);

    /**
     * Update camera position based on IMU data
     * Applies any configured transformations/calibrations
     * @param imu_data IMU data to use for positioning
     * @return true if successful, false otherwise
     */
    bool updateCameraPosition(const IMUData& imu_data);

    /**
     * Get IMU reader
     * @return Pointer to IMU reader, nullptr if not initialized
     */
    IMUReader* getIMUReader() {
        return imu_reader_.get();
    }

    /**
     * Get camera controller
     * @return Pointer to camera controller, nullptr if not initialized
     */
    CameraControl* getCameraControl() {
        return camera_control_.get();
    }

    /**
     * Check if all systems are initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const { return initialized_; }

    /**
     * Check if IMU is running
     * @return true if running, false otherwise
     */
    bool isIMURunning() const {
        return imu_reader_ && imu_reader_->isRunning();
    }

    /**
     * Check if camera is connected
     * @return true if connected, false otherwise
     */
    bool isCameraConnected() const {
        return camera_control_ && camera_control_->isConnected();
    }

private:
    HardwareManager() = default;
    
    ~HardwareManager() {
        shutdown();
    }

    // Components
    std::unique_ptr<IMUReader> imu_reader_;
    std::unique_ptr<CameraControl> camera_control_;
    
    // State
    bool initialized_{false};
    
    // Configuration
    Config config_;
    
    // Logging
    utils::Logger& logger_ = utils::Logger::getInstance();
};

}  // namespace hardware

#endif  // HARDWARE_MANAGER_H
