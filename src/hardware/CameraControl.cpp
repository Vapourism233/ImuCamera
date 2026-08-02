#include "CameraControl.h"
#include "HCNetSDK.h"
#include <cstring>
#include <sstream>

namespace hardware {

CameraControl::CameraControl(const std::string& device_ip,
                             uint16_t port,
                             const std::string& username,
                             const std::string& password,
                             uint32_t channel)
    : device_ip_(device_ip), port_(port), username_(username), 
      password_(password), channel_(channel) {
    logger_.infof("CameraControl created: %s:%u channel=%u", device_ip_.c_str(), port_, channel_);
}

CameraControl::~CameraControl() {
    disconnect();
    logger_.debug("CameraControl destroyed");
}

bool CameraControl::connect() {
    if (connected_) {
        logger_.warning("Camera is already connected");
        return true;
    }

    // Prepare login info
    NET_DVR_USER_LOGIN_INFO login_info = {0};
    NET_DVR_DEVICEINFO_V40 device_info = {0};

    login_info.bUseAsynLogin = false;
    login_info.wPort = port_;

    // Copy device info
    strncpy((char*)login_info.sDeviceAddress, device_ip_.c_str(), 
            sizeof(login_info.sDeviceAddress) - 1);
    strncpy((char*)login_info.sUserName, username_.c_str(), 
            sizeof(login_info.sUserName) - 1);
    strncpy((char*)login_info.sPassword, password_.c_str(), 
            sizeof(login_info.sPassword) - 1);

    // Attempt login
    user_id_ = NET_DVR_Login_V40(&login_info, &device_info);
    if (user_id_ < 0) {
        std::stringstream ss;
        ss << "Failed to login to camera (error code: " << NET_DVR_GetLastError() << ")";
        setError(ss.str());
        return false;
    }

    // Configure JPEG capture mode
    if (!NET_DVR_SetCapturePictureMode(BMP_MODE)) {
        std::stringstream ss;
        ss << "Failed to set capture picture mode (error code: " << NET_DVR_GetLastError() << ")";
        setError(ss.str());
        NET_DVR_Logout(user_id_);
        user_id_ = -1;
        return false;
    }

    connected_ = true;
    logger_.infof("Connected to camera successfully (user_id: %ld)", user_id_);
    return true;
}

bool CameraControl::disconnect() {
    if (!connected_) {
        return true;
    }

    if (user_id_ >= 0) {
        if (NET_DVR_Logout(user_id_)) {
            logger_.infof("Logged out from camera (user_id: %ld)", user_id_);
        } else {
            logger_.warningf("Failed to logout from camera (error code: %d)", 
                            NET_DVR_GetLastError());
        }
        user_id_ = -1;
    }

    connected_ = false;
    return true;
}

bool CameraControl::pan(uint32_t speed, uint32_t direction) {
    if (!connected_) {
        setError("Camera not connected");
        return false;
    }

    if (speed > 127) {
        setError("Pan speed out of range (0-127)");
        return false;
    }

    if (direction > 1) {
        setError("Pan direction invalid (0=left, 1=right)");
        return false;
    }

    // TODO: Implement pan control using HIKvision SDK
    // This requires knowledge of the specific SDK control commands
    logger_.debugf("Pan command: speed=%u, direction=%u", speed, direction);
    
    return true;
}

bool CameraControl::tilt(uint32_t speed, uint32_t direction) {
    if (!connected_) {
        setError("Camera not connected");
        return false;
    }

    if (speed > 127) {
        setError("Tilt speed out of range (0-127)");
        return false;
    }

    if (direction > 1) {
        setError("Tilt direction invalid (0=up, 1=down)");
        return false;
    }

    // TODO: Implement tilt control using HIKvision SDK
    logger_.debugf("Tilt command: speed=%u, direction=%u", speed, direction);
    
    return true;
}

bool CameraControl::zoom(uint32_t speed, uint32_t direction) {
    if (!connected_) {
        setError("Camera not connected");
        return false;
    }

    if (speed > 127) {
        setError("Zoom speed out of range (0-127)");
        return false;
    }

    if (direction > 1) {
        setError("Zoom direction invalid (0=out, 1=in)");
        return false;
    }

    // TODO: Implement zoom control using HIKvision SDK
    logger_.debugf("Zoom command: speed=%u, direction=%u", speed, direction);
    
    return true;
}

bool CameraControl::setPosition(uint32_t pan_position, uint32_t tilt_position) {
    if (!connected_) {
        setError("Camera not connected");
        return false;
    }

    // TODO: Implement absolute position control using HIKvision SDK
    logger_.debugf("Set position: pan=%u, tilt=%u", pan_position, tilt_position);
    
    return true;
}

bool CameraControl::stop() {
    if (!connected_) {
        setError("Camera not connected");
        return false;
    }

    // TODO: Implement stop command using HIKvision SDK
    logger_.debug("Stop command sent");
    
    return true;
}

bool CameraControl::captureFrame(const std::string& output_path) {
    if (!connected_) {
        setError("Camera not connected");
        return false;
    }

    // TODO: Implement frame capture using HIKvision SDK
    logger_.debugf("Capture frame to: %s", output_path.c_str());
    
    return true;
}

void CameraControl::setError(const std::string& error) {
    last_error_ = error;
    logger_.error(error);
}

}  // namespace hardware
