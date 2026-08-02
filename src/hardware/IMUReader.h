#ifndef IMU_READER_H
#define IMU_READER_H

#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include "../utils/Logger.h"
#include "../utils/ThreadSafeQueue.h"
#include "IMUData.h"

namespace hardware {

/**
 * @brief IMU Reader - handles serial communication with IMU sensor
 * 
 * This class manages:
 * - Serial port connection/disconnection
 * - Reading IMU data in a separate thread
 * - Thread-safe data delivery via queue
 * 
 * Example usage:
 * @code
 * auto imu = std::make_unique<IMUReader>("/dev/ttyUSB0", 115200);
 * if (imu->start()) {
 *     IMUData data;
 *     if (imu->tryGetData(data)) {
 *         std::cout << "Pitch: " << data.pitch << std::endl;
 *     }
 * }
 * imu->stop();
 * @endcode
 */
class IMUReader {
public:
    /**
     * Constructor
     * @param device Serial device path (e.g., "/dev/ttyUSB0")
     * @param baudrate Serial communication baudrate (default: 115200)
     */
    IMUReader(const std::string& device, uint32_t baudrate = 115200);
    
    /**
     * Destructor - ensures resources are cleaned up
     */
    ~IMUReader();

    /**
     * Start the IMU reader thread
     * @return true if successfully started, false otherwise
     */
    bool start();

    /**
     * Stop the IMU reader thread and close serial port
     * @return true if successfully stopped, false otherwise
     */
    bool stop();

    /**
     * Check if reader is currently running
     * @return true if running, false otherwise
     */
    bool isRunning() const { return running_.load(); }

    /**
     * Try to get the latest IMU data (non-blocking)
     * @param data Reference to store the IMU data
     * @return true if data was available, false if queue is empty
     */
    bool tryGetData(IMUData& data) {
        return data_queue_.tryPop(data);
    }

    /**
     * Wait for IMU data (blocking)
     * @param data Reference to store the IMU data
     */
    void waitForData(IMUData& data) {
        data_queue_.waitAndPop(data);
    }

    /**
     * Get number of pending data frames in queue
     * @return Queue size
     */
    size_t getQueueSize() const {
        return data_queue_.size();
    }

private:
    /**
     * Main reader thread function
     * Reads from serial port and parses IMU data
     */
    void readerThreadFunc();

    /**
     * Configure serial port settings
     * @param fd File descriptor of serial port
     * @return true if successful, false otherwise
     */
    bool configureSerialPort(int fd);

    /**
     * Parse IMU data from buffer
     * Currently a placeholder - should parse actual protocol
     * @param buffer Raw data buffer
     * @param length Buffer length
     * @return Parsed IMU data
     */
    IMUData parseIMUData(const char* buffer, size_t length);

    // Configuration
    std::string device_;
    uint32_t baudrate_;
    
    // Runtime state
    std::atomic<bool> running_{false};
    std::unique_ptr<std::thread> reader_thread_;
    
    // Data sharing
    utils::ThreadSafeQueue<IMUData> data_queue_;
    
    // Logging
    utils::Logger& logger_ = utils::Logger::getInstance();
};

}  // namespace hardware

#endif  // IMU_READER_H
