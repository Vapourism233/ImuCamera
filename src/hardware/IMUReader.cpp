#include "IMUReader.h"
#include "../utils/FileDescriptor.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <iostream>

namespace hardware {

IMUReader::IMUReader(const std::string& device, uint32_t baudrate)
    : device_(device), baudrate_(baudrate) {
    logger_.debug("IMUReader created for device: " + device);
}

IMUReader::~IMUReader() {
    stop();
    logger_.debug("IMUReader destroyed");
}

bool IMUReader::start() {
    if (running_.load()) {
        logger_.warning("IMUReader is already running");
        return false;
    }

    running_ = true;
    try {
        reader_thread_ = std::make_unique<std::thread>(&IMUReader::readerThreadFunc, this);
        logger_.info("IMUReader started successfully");
        return true;
    } catch (const std::exception& e) {
        logger_.errorf("Failed to start IMUReader: %s", e.what());
        running_ = false;
        return false;
    }
}

bool IMUReader::stop() {
    if (!running_.load()) {
        logger_.warning("IMUReader is not running");
        return false;
    }

    running_ = false;
    
    if (reader_thread_ && reader_thread_->joinable()) {
        reader_thread_->join();
        reader_thread_.reset();
    }
    
    logger_.info("IMUReader stopped");
    return true;
}

void IMUReader::readerThreadFunc() {
    utils::FileDescriptor fd;
    char buffer[512];
    
    try {
        // Open serial port
        fd = utils::FileDescriptor(open(device_.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK)); // Use FileDescriptor RAII wrapper
        if(!fd.valid()) {
            logger_.errorf("Failed to open serial port: %s", device_.c_str());
            running_ = false;
            return;
        } else {
            logger_.infof("Serial port opened: %s", device_.c_str());
        }
        // Configure serial port
        if (!configureSerialPort(fd)) {
            logger_.error("Failed to configure serial port");
            // FileDescriptor will automatically close the fd when it goes out of scope
            running_ = false;
            return;
        }

        logger_.infof("Serial port opened and configured: %s", device_.c_str());

        // Read loop
        while (running_.load()) {
            memset(buffer, 0, sizeof(buffer));
            
            // Set non-blocking read timeout (100ms)
            fd_set readfds;
            struct timeval tv;
            FD_ZERO(&readfds);
            FD_SET(fd.get(), &readfds);
            tv.tv_sec = 0;
            tv.tv_usec = 100000;  // 100ms timeout

            int select_result = select(fd.get() + 1, &readfds, nullptr, nullptr, &tv);
            
            if (select_result > 0 && FD_ISSET(fd.get(), &readfds)) {
                ssize_t n = read(fd.get(), buffer, sizeof(buffer) - 1);
                if (n > 0) {
                    buffer[n] = '\0';
                    IMUData data = parseIMUData(buffer, n);
                    
                    // Add timestamp if not already set
                    if (data.timestamp == 0) {
                        data.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch()
                        ).count();
                    }
                    
                    data_queue_.push(data);
                    logger_.debugf("IMU data received - Pitch: %.2f, Yaw: %.2f", data.pitch, data.yaw);
                } else if (n < 0) {
                    logger_.error("Error reading from serial port");
                }
            } else if (select_result < 0) {
                logger_.error("select() error on serial port");
                break;
            }
            // select_result == 0 means timeout, just continue
        }

    } catch (const std::exception& e) {
        logger_.errorf("Exception in reader thread: %s", e.what());
        running_ = false;
    }

    // Cleanup
    // FileDescriptor will automatically close the fd when it goes out of scope
}

bool IMUReader::configureSerialPort(const utils::FileDescriptor& fd) {
    if (!fd.valid()) {
        return false;
    }

    struct termios options;
    
    // Get current settings
    if (tcgetattr(fd.get(), &options) < 0) {
        logger_.error("tcgetattr failed");
        return false;
    }

    // Set baudrate
    speed_t baud;
    switch (baudrate_) {
        case 9600:    baud = B9600; break;
        case 19200:   baud = B19200; break;
        case 38400:   baud = B38400; break;
        case 115200:  baud = B115200; break;
        case 230400:  baud = B230400; break;
        default:
            logger_.errorf("Unsupported baudrate: %u", baudrate_);
            return false;
    }

    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    // 8N1: 8 data bits, no parity, 1 stop bit
    options.c_cflag &= ~PARENB;           // No parity
    options.c_cflag &= ~CSTOPB;           // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;               // 8 data bits
    options.c_cflag |= CREAD | CLOCAL;   // Enable receiver, local mode

    // Input flags
    options.c_iflag &= ~(IXON | IXOFF | IXANY);  // No software flow control
    options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Output flags
    options.c_oflag &= ~OPOST;

    // Control characters
    options.c_cc[VMIN] = 0;   // Non-blocking read
    options.c_cc[VTIME] = 10; // 1 second timeout

    // Apply settings
    if (tcsetattr(fd.get(), TCSANOW, &options) < 0) {
        logger_.error("tcsetattr failed");
        return false;
    }

    // Flush buffers
    tcflush(fd.get(), TCIOFLUSH);

    logger_.infof("Serial port configured: %u baud, 8N1", baudrate_);
    return true;
}

IMUData IMUReader::parseIMUData(const char* buffer, size_t length) {
    IMUData data;
    
    // TODO: Implement actual protocol parsing based on IMU manufacturer specification
    // This is a placeholder implementation
    // For TranducerM IMU, you need to parse the actual protocol format
    
    // Example: if using simple CSV format "pitch,yaw,roll"
    // float pitch = 0, yaw = 0, roll = 0;
    // if (sscanf(buffer, "%f,%f,%f", &pitch, &yaw, &roll) == 3) {
    //     data.pitch = pitch;
    //     data.yaw = yaw;
    //     data.roll = roll;
    // }
    
    logger_.debugf("Parsing IMU data from buffer (length: %zu)", length);
    return data;
}

}  // namespace hardware
