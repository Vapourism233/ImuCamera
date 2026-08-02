#include "HardwareManager.h"

namespace hardware {

bool HardwareManager::init(const Config& config) {
    if (initialized_) {
        logger_.warning("HardwareManager is already initialized");
        return true;
    }

    config_ = config;

    // Initialize logging
    logger_.init(config.log_file, config.log_level);
    logger_.info("====== HardwareManager Initialization Starting ======");

    try {
        // Create IMU reader
        imu_reader_ = std::make_unique<IMUReader>(config.imu_device, config.imu_baudrate);
        logger_.infof("IMU Reader created for device: %s", config.imu_device.c_str());

        // Create camera controller
        camera_control_ = std::make_unique<CameraControl>(
            config.camera_ip,
            config.camera_port,
            config.camera_username,
            config.camera_password,
            config.camera_channel
        );
        logger_.infof("Camera Controller created for: %s:%u", 
                     config.camera_ip.c_str(), config.camera_port);

        initialized_ = true;
        logger_.info("====== HardwareManager Initialization Complete ======");
        return true;

    } catch (const std::exception& e) {
        logger_.errorf("Exception during initialization: %s", e.what());
        initialized_ = false;
        return false;
    }
}

bool HardwareManager::shutdown() {
    if (!initialized_) {
        return true;
    }

    logger_.info("====== HardwareManager Shutdown Starting ======");

    // Stop IMU if running
    if (imu_reader_) {
        if (imu_reader_->isRunning()) {
            stopIMU();
        }
        imu_reader_.reset();
    }

    // Disconnect camera if connected
    if (camera_control_) {
        if (camera_control_->isConnected()) {
            disconnectCamera();
        }
        camera_control_.reset();
    }

    initialized_ = false;
    logger_.info("====== HardwareManager Shutdown Complete ======");
    return true;
}

bool HardwareManager::startIMU() {
    if (!initialized_) {
        logger_.error("HardwareManager not initialized");
        return false;
    }

    if (!imu_reader_) {
        logger_.error("IMU Reader not created");
        return false;
    }

    if (imu_reader_->isRunning()) {
        logger_.warning("IMU Reader is already running");
        return true;
    }

    logger_.info("Starting IMU Reader...");
    bool result = imu_reader_->start();
    
    if (result) {
        logger_.info("IMU Reader started successfully");
    } else {
        logger_.error("Failed to start IMU Reader");
    }
    
    return result;
}

bool HardwareManager::stopIMU() {
    if (!imu_reader_) {
        return true;
    }

    if (!imu_reader_->isRunning()) {
        return true;
    }

    logger_.info("Stopping IMU Reader...");
    bool result = imu_reader_->stop();
    
    if (result) {
        logger_.info("IMU Reader stopped successfully");
    } else {
        logger_.error("Failed to stop IMU Reader");
    }
    
    return result;
}

bool HardwareManager::connectCamera() {
    if (!initialized_) {
        logger_.error("HardwareManager not initialized");
        return false;
    }

    if (!camera_control_) {
        logger_.error("Camera Control not created");
        return false;
    }

    if (camera_control_->isConnected()) {
        logger_.warning("Camera is already connected");
        return true;
    }

    logger_.info("Connecting to camera...");
    bool result = camera_control_->connect();
    
    if (result) {
        logger_.info("Camera connected successfully");
    } else {
        logger_.error("Failed to connect to camera: " + camera_control_->getLastError());
    }
    
    return result;
}

bool HardwareManager::disconnectCamera() {
    if (!camera_control_) {
        return true;
    }

    if (!camera_control_->isConnected()) {
        return true;
    }

    logger_.info("Disconnecting from camera...");
    bool result = camera_control_->disconnect();
    
    if (result) {
        logger_.info("Camera disconnected successfully");
    } else {
        logger_.error("Failed to disconnect from camera");
    }
    
    return result;
}

bool HardwareManager::getLatestIMUData(IMUData& data) {
    if (!imu_reader_) {
        return false;
    }

    return imu_reader_->tryGetData(data);
}

bool HardwareManager::updateCameraPosition(const IMUData& imu_data) {
    if (!camera_control_ || !camera_control_->isConnected()) {
        return false;
    }

    // TODO: Implement calibration and transformation logic
    // Convert IMU angles to camera PTZ commands
    // This depends on physical mounting and calibration
    
    // Example: map pitch to tilt, yaw to pan
    // Requires calibration matrix to be determined experimentally
    
    logger_.debugf("Updating camera position - Pitch: %.2f°, Yaw: %.2f°", 
                  imu_data.pitch, imu_data.yaw);

    // Placeholder implementation
    return true;
}

}  // namespace hardware
