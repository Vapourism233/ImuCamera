#ifndef IMU_DATA_H
#define IMU_DATA_H

#include <cstdint>

namespace hardware {

/**
 * @brief IMU sensor data structure
 * Stores orientation data (Roll, Pitch, Yaw) from IMU sensor
 */
struct IMUData {
    float pitch;        ///< Pitch angle in degrees
    float yaw;          ///< Yaw angle in degrees
    float roll;         ///< Roll angle in degrees
    uint64_t timestamp; ///< Timestamp in milliseconds

    IMUData() : pitch(0), yaw(0), roll(0), timestamp(0) {}
    
    IMUData(float p, float y, float r, uint64_t ts) 
        : pitch(p), yaw(y), roll(r), timestamp(ts) {}
};

}  // namespace hardware

#endif  // IMU_DATA_H
