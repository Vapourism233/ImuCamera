#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include <string>
#include <memory>
#include <cstdint>
#include "../utils/Logger.h"

namespace hardware {

/**
 * @brief Camera PTZ control interface
 * Handles pan, tilt, zoom operations via HIKvision SDK
 */
class CameraControl {
public:
    /**
     * Constructor
     * @param device_ip Camera IP address
     * @param port Camera port (default: 8000)
     * @param username Username for authentication
     * @param password Password for authentication
     * @param channel Channel number (default: 1)
     */
    CameraControl(const std::string& device_ip, 
                  uint16_t port,
                  const std::string& username,
                  const std::string& password,
                  uint32_t channel = 1);
    
    /**
     * Destructor - ensures connection is closed
     */
    ~CameraControl();

    /**
     * Connect to the camera
     * @return true if successfully connected, false otherwise
     */
    bool connect();

    /**
     * Disconnect from the camera
     * @return true if successfully disconnected, false otherwise
     */
    bool disconnect();

    /**
     * Check if connected to camera
     * @return true if connected, false otherwise
     */
    bool isConnected() const { return connected_; }

    /**
     * Pan camera (rotate left/right)
     * @param speed Speed value (0-127)
     * @param direction 0 = left, 1 = right
     * @return true if successful, false otherwise
     */
    bool pan(uint32_t speed, uint32_t direction);

    /**
     * Tilt camera (rotate up/down)
     * @param speed Speed value (0-127)
     * @param direction 0 = up, 1 = down
     * @return true if successful, false otherwise
     */
    bool tilt(uint32_t speed, uint32_t direction);

    /**
     * Zoom camera (in/out)
     * @param speed Speed value (0-127)
     * @param direction 0 = zoom out, 1 = zoom in
     * @return true if successful, false otherwise
     */
    bool zoom(uint32_t speed, uint32_t direction);

    /**
     * Set absolute position (pan + tilt)
     * @param pan_position Pan position (0-3600, represents 0-360 degrees)
     * @param tilt_position Tilt position (0-900, represents 0-90 degrees)
     * @return true if successful, false otherwise
     */
    bool setPosition(uint32_t pan_position, uint32_t tilt_position);

    /**
     * Stop all movement
     * @return true if successful, false otherwise
     */
    bool stop();

    /**
     * Capture a JPEG frame from the camera
     * @param output_path Path to save the JPEG image
     * @return true if successful, false otherwise
     */
    bool captureFrame(const std::string& output_path);

    /**
     * Get the last error message
     * @return Error message string
     */
    std::string getLastError() const { return last_error_; }

private:
    // Connection settings
    std::string device_ip_;
    uint16_t port_;
    std::string username_;
    std::string password_;
    uint32_t channel_;
    
    // Connection state
    bool connected_{false};
    long user_id_{-1};  // HIKvision SDK user handle
    
    // Error tracking
    std::string last_error_;
    
    // Logging
    utils::Logger& logger_ = utils::Logger::getInstance();

    /**
     * Internal helper to set last error message and log it
     */
    void setError(const std::string& error);
};

}  // namespace hardware

#endif  // CAMERA_CONTROL_H
