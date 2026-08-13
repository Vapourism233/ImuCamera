#ifndef MOCK_CAMERA_CONTROL_H
#define MOCK_CAMERA_CONTROL_H

#include <string>
#include <memory>
#include <cstdint>
#include <vector>
#include "../src/utils/Logger.h"

namespace hardware {

/**
 * @brief Mock Camera Control - simulates PTZ camera behavior
 * 
 * This class provides simulated camera control for testing without hardware.
 * It logs all operations and can report position/status.
 * 
 * Usage exactly same as CameraControl - just use MockCameraControl instead
 */
class MockCameraControl {
public:
    struct OperationLog {
        std::string operation;
        uint32_t timestamp;
        std::string parameters;
    };

    /**
     * Constructor
     * @param device_ip Ignored (for API compatibility)
     * @param port Ignored (for API compatibility)
     * @param username Ignored (for API compatibility)
     * @param password Ignored (for API compatibility)
     * @param channel Ignored (for API compatibility)
     */
    MockCameraControl(const std::string& device_ip = "192.168.1.1", 
                      uint16_t port = 8000,
                      const std::string& username = "admin",
                      const std::string& password = "password",
                      uint32_t channel = 1);
    
    /**
     * Destructor
     */
    ~MockCameraControl();

    /**
     * Connect to the camera (always succeeds in mock)
     * @return true
     */
    bool connect();

    /**
     * Disconnect from the camera (always succeeds in mock)
     * @return true
     */
    bool disconnect();

    /**
     * Check if connected to camera
     * @return true if mock is connected
     */
    bool isConnected() const { return connected_; }

    /**
     * Pan camera (rotate left/right)
     * @param speed Speed value (0-127)
     * @param direction 0=left, 1=right
     * @return true if successful
     */
    bool pan(uint8_t speed, uint8_t direction);

    /**
     * Tilt camera (rotate up/down)
     * @param speed Speed value (0-127)
     * @param direction 0=down, 1=up
     * @return true if successful
     */
    bool tilt(uint8_t speed, uint8_t direction);

    /**
     * Zoom camera
     * @param speed Speed value (0-127)
     * @param direction 0=zoom out, 1=zoom in
     * @return true if successful
     */
    bool zoom(uint8_t speed, uint8_t direction);

    /**
     * Stop all movement
     * @return true if successful
     */
    bool stop();

    /**
     * Set camera to absolute position
     * @param pan_pos Pan position (-3600 to 3600 = -360 to 360 degrees)
     * @param tilt_pos Tilt position (-900 to 900 = -90 to 90 degrees)
     * @return true if successful
     */
    bool setPosition(int32_t pan_pos, int32_t tilt_pos);

    /**
     * Get current pan position
     * @return Pan position in 0.1 degree units
     */
    int32_t getPanPosition() const { return pan_position_; }

    /**
     * Get current tilt position
     * @return Tilt position in 0.1 degree units
     */
    int32_t getTiltPosition() const { return tilt_position_; }

    /**
     * Capture a frame
     * @param output_path Path where to save the JPEG (ignored in mock)
     * @return true if successful
     */
    bool captureFrame(const std::string& output_path);

    /**
     * Get operation history for testing/debugging
     * @return Vector of all operations performed
     */
    const std::vector<OperationLog>& getOperationHistory() const;

    /**
     * Clear operation history
     */
    void clearHistory();

    /**
     * Simulate camera error on next operation
     */
    void simulateError(bool should_error = true);

private:
    std::string device_ip_;
    uint16_t port_;
    std::string username_;
    std::string password_;
    uint32_t channel_;
    
    bool connected_{false};
    bool simulate_error_{false};
    
    // Simulated camera state
    int32_t pan_position_{0};    // 0.1 degree units
    int32_t tilt_position_{0};   // 0.1 degree units
    float zoom_level_{1.0f};
    
    // Operation history
    std::vector<OperationLog> operation_history_;
    
    // Helper methods
    void logOperation(const std::string& operation, const std::string& params);
    bool checkConnection();
};

} // namespace hardware

#endif // MOCK_CAMERA_CONTROL_H
