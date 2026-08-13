#include "MockCameraControl.h"
#include <sstream>
#include <chrono>

namespace hardware {

MockCameraControl::MockCameraControl(const std::string& device_ip,
                                     uint16_t port,
                                     const std::string& username,
                                     const std::string& password,
                                     uint32_t channel)
    : device_ip_(device_ip), port_(port), username_(username),
      password_(password), channel_(channel) {
    utils::Logger::getInstance().info(
        "MockCameraControl created for " + device_ip + ":" + std::to_string(port));
}

MockCameraControl::~MockCameraControl() {
    disconnect();
    utils::Logger::getInstance().info("MockCameraControl destroyed");
}

bool MockCameraControl::connect() {
    if (simulate_error_) {
        utils::Logger::getInstance().error("MockCameraControl: simulated connection error");
        simulate_error_ = false;
        return false;
    }
    
    connected_ = true;
    logOperation("connect", "Connected to camera");
    utils::Logger::getInstance().info("MockCameraControl connected");
    return true;
}

bool MockCameraControl::disconnect() {
    if (!connected_) {
        return true;
    }
    
    connected_ = false;
    logOperation("disconnect", "Disconnected from camera");
    utils::Logger::getInstance().info("MockCameraControl disconnected");
    return true;
}

bool MockCameraControl::pan(uint8_t speed, uint8_t direction) {
    if (!checkConnection()) return false;
    
    if (simulate_error_) {
        simulate_error_ = false;
        return false;
    }
    
    std::string dir_str = (direction == 0) ? "left" : "right";
    std::ostringstream oss;
    oss << "speed=" << (int)speed << ", direction=" << dir_str;
    logOperation("pan", oss.str());
    
    // Simulate movement
    int movement = (direction == 0) ? -speed * 10 : speed * 10;
    pan_position_ = std::max(-3600, std::min(3600, pan_position_ + movement));
    
    utils::Logger::getInstance().debug("Pan: " + dir_str + " at speed " + std::to_string(speed));
    return true;
}

bool MockCameraControl::tilt(uint8_t speed, uint8_t direction) {
    if (!checkConnection()) return false;
    
    if (simulate_error_) {
        simulate_error_ = false;
        return false;
    }
    
    std::string dir_str = (direction == 0) ? "down" : "up";
    std::ostringstream oss;
    oss << "speed=" << (int)speed << ", direction=" << dir_str;
    logOperation("tilt", oss.str());
    
    // Simulate movement
    int movement = (direction == 0) ? -speed * 10 : speed * 10;
    tilt_position_ = std::max(-900, std::min(900, tilt_position_ + movement));
    
    utils::Logger::getInstance().debug("Tilt: " + dir_str + " at speed " + std::to_string(speed));
    return true;
}

bool MockCameraControl::zoom(uint8_t speed, uint8_t direction) {
    if (!checkConnection()) return false;
    
    if (simulate_error_) {
        simulate_error_ = false;
        return false;
    }
    
    std::string dir_str = (direction == 0) ? "out" : "in";
    std::ostringstream oss;
    oss << "speed=" << (int)speed << ", direction=" << dir_str;
    logOperation("zoom", oss.str());
    
    // Simulate zoom (1.0 = no zoom, max 4.0)
    float zoom_change = (direction == 0) ? -speed * 0.01f : speed * 0.01f;
    zoom_level_ = std::max(1.0f, std::min(4.0f, zoom_level_ + zoom_change));
    
    utils::Logger::getInstance().debug("Zoom: " + dir_str + " at speed " + std::to_string(speed));
    return true;
}

bool MockCameraControl::stop() {
    if (!checkConnection()) return false;
    
    logOperation("stop", "All movement stopped");
    utils::Logger::getInstance().debug("Camera stopped");
    return true;
}

bool MockCameraControl::setPosition(int32_t pan_pos, int32_t tilt_pos) {
    if (!checkConnection()) return false;
    
    if (simulate_error_) {
        simulate_error_ = false;
        return false;
    }
    
    // Validate ranges
    if (pan_pos < -3600 || pan_pos > 3600 || tilt_pos < -900 || tilt_pos > 900) {
        utils::Logger::getInstance().warning("MockCameraControl: invalid position");
        return false;
    }
    
    pan_position_ = pan_pos;
    tilt_position_ = tilt_pos;
    
    std::ostringstream oss;
    oss << "pan=" << pan_pos << ", tilt=" << tilt_pos;
    logOperation("setPosition", oss.str());
    
    utils::Logger::getInstance().debug("Position set: " + oss.str());
    return true;
}

bool MockCameraControl::captureFrame(const std::string& output_path) {
    if (!checkConnection()) return false;
    
    if (simulate_error_) {
        simulate_error_ = false;
        return false;
    }
    
    logOperation("captureFrame", "path=" + output_path);
    utils::Logger::getInstance().info("Frame captured to: " + output_path);
    return true;
}

const std::vector<MockCameraControl::OperationLog>& 
MockCameraControl::getOperationHistory() const {
    return operation_history_;
}

void MockCameraControl::clearHistory() {
    operation_history_.clear();
}

void MockCameraControl::simulateError(bool should_error) {
    simulate_error_ = should_error;
    if (should_error) {
        utils::Logger::getInstance().debug("Simulating error on next operation");
    }
}

void MockCameraControl::logOperation(const std::string& operation, 
                                     const std::string& params) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    
    OperationLog log;
    log.operation = operation;
    log.timestamp = timestamp;
    log.parameters = params;
    
    operation_history_.push_back(log);
}

bool MockCameraControl::checkConnection() {
    if (!connected_) {
        utils::Logger::getInstance().warning(
            "MockCameraControl: operation attempted on disconnected camera");
        return false;
    }
    return true;
}

} // namespace hardware
