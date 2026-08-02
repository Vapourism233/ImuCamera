# Hardware Management Module - Quick Start Guide

## 📦 File Structure

The refactored hardware management module has been added to the `src/` directory:

```
ImuCamera/
├── src/
│   ├── utils/
│   │   ├── Logger.h              # Logging system
│   │   └── ThreadSafeQueue.h     # Thread-safe queue
│   ├── hardware/
│   │   ├── IMUData.h             # Data structure
│   │   ├── IMUReader.h/.cpp      # IMU serial reading
│   │   ├── CameraControl.h/.cpp  # Camera PTZ control
│   │   └── HardwareManager.h/.cpp# Hardware coordinator
│   ├── config/
│   │   ├── ConfigManager.h/.cpp  # Configuration management
│   └── app/
│       ├── Application.h/.cpp    # Main application
│       └── main_new.cpp          # New entry point
├── config.ini                     # Configuration file
└── old/                           # Legacy code (archived)
```

## 🚀 Quick Start in 3 Steps

### Step 1: Configure System (5 minutes)

Edit \`config.ini\` and set your hardware parameters:

\`\`\`ini
[imu]
device = /dev/ttyUSB0      # Change to your IMU serial port
baudrate = 115200

[camera]
ip = 192.168.1.8           # Change to your camera IP
port = 8000
username = admin
password = a1234567        # Change to your password
channel = 1

[logging]
file = ./logs/imu_camera.log
level = INFO

[vision]
save_regions = false
show_steps = false
\`\`\`

### Step 2: Compile (10 minutes)

Edit \`CMakeLists.txt\` and add the new source files:

\`\`\`cmake
# New hardware management module sources
set(NEW_SOURCES
    src/hardware/IMUReader.cpp
    src/hardware/CameraControl.cpp
    src/hardware/HardwareManager.cpp
    src/config/ConfigManager.cpp
    src/app/Application.cpp
    src/main_new.cpp
)

add_executable(imu_camera_new \${NEW_SOURCES} \${OLD_SOURCES})
target_include_directories(imu_camera_new PRIVATE src/)
target_link_libraries(imu_camera_new pthread \${OpenCV_LIBS} \${OTHER_LIBS})
\`\`\`

Build:
\`\`\`bash
mkdir -p build
cd build
cmake ..
make imu_camera_new
\`\`\`

### Step 3: Run (5 minutes)

\`\`\`bash
# Using default configuration
./imu_camera_new

# Using custom configuration
./imu_camera_new /path/to/config.ini
\`\`\`

## 💡 Usage Examples

### Basic Usage

\`\`\`cpp
#include "src/app/Application.h"

int main() {
    app::Application app;
    
    if (\!app.init("config.ini")) {
        std::cerr << "Failed to initialize" << std::endl;
        return 1;
    }
    
    int exit_code = app.run();
    app.shutdown();
    
    return exit_code;
}
\`\`\`

### Direct Hardware Manager Usage

\`\`\`cpp
#include "src/hardware/HardwareManager.h"
#include "src/utils/Logger.h"

int main() {
    auto& logger = utils::Logger::getInstance();
    logger.init("app.log");
    
    auto& hwm = hardware::HardwareManager::getInstance();
    
    hardware::HardwareManager::Config config{
        .imu_device = "/dev/ttyUSB0",
        .imu_baudrate = 115200,
        .camera_ip = "192.168.1.8",
        .camera_port = 8000,
        .camera_username = "admin",
        .camera_password = "password"
    };
    
    if (\!hwm.init(config)) {
        logger.error("Init failed");
        return 1;
    }
    
    hwm.startIMU();
    hwm.connectCamera();
    
    // Get IMU data
    hardware::IMUData data;
    if (hwm.getLatestIMUData(data)) {
        logger.infof("IMU: pitch=%.1f, yaw=%.1f", data.pitch, data.yaw);
    }
    
    hwm.shutdown();
    return 0;
}
\`\`\`

## 🔍 Debugging Tips

### Enable Verbose Logging

Modify \`config.ini\`:
\`\`\`ini
[logging]
level = DEBUG      # Change to DEBUG for detailed output
\`\`\`

### Check Serial Port

\`\`\`bash
# List available serial ports
ls /dev/ttyUSB*

# Test connection
cat < /dev/ttyUSB0
\`\`\`

### Monitor Threads

\`\`\`bash
# Real-time thread monitoring
watch -n 1 'ps -eLf | grep imu_camera_new'

# Check thread count
cat /proc/<pid>/status | grep Threads
\`\`\`

## ⚠️ Troubleshooting

### Cannot Open Serial Port

**Error**: \`Failed to open serial device: /dev/ttyUSB0\`

**Solutions**:
- Check device connection: \`ls /dev/ttyUSB*\`
- Check permissions: \`ls -l /dev/ttyUSB0\`
- Grant permissions: \`sudo chmod 666 /dev/ttyUSB0\`
- Or add user to dialout group: \`sudo usermod -aG dialout $USER\`

### Camera Connection Failed

**Error**: \`Failed to login to camera (error code: xxx)\`

**Solutions**:
- Verify IP address: \`ping 192.168.1.8\`
- Check firewall: \`sudo ufw allow 8000\`
- Verify credentials are correct
- Check if camera is online

### Thread Exits Too Quickly

**Problem**: Application exits immediately

**Solution**:
- Check log file: \`tail -f ./logs/imu_camera.log\`
- Increase log level to DEBUG
- Check configuration file syntax

## 📊 Performance Targets

Based on design specifications:

| Metric | Target | Note |
|--------|--------|------|
| IMU Latency | < 10ms | From serial read to main thread |
| Processing Frame Rate | ~30 FPS | Configurable |
| Memory Usage | < 50MB | Basic runtime |
| CPU Usage | 30-50% single core | ~10-15% on 4-core system |
| Logging Overhead | < 1% | At INFO level |

## 🧪 Unit Testing

Create \`tests/test_hardware.cpp\`:

\`\`\`cpp
#include <gtest/gtest.h>
#include "src/utils/Logger.h"
#include "src/utils/ThreadSafeQueue.h"

TEST(ThreadSafeQueue, BasicPushPop) {
    utils::ThreadSafeQueue<int> queue;
    queue.push(42);
    
    int value;
    ASSERT_TRUE(queue.tryPop(value));
    ASSERT_EQ(value, 42);
}

TEST(Logger, Initialization) {
    auto& logger = utils::Logger::getInstance();
    ASSERT_NO_THROW(logger.init("test.log"));
}
\`\`\`

## Recommended Reading Order

1. ⭐ **QUICKSTART.md** - Quick overview (this file)
2. **IMPROVEMENTS.md** - Understand the improvements
3. **HARDWARE_MODULE_DESIGN.md** - Deep dive into architecture
4. **REFACTORING_SUMMARY.md** - Integration roadmap

---

**Happy refactoring\!** 🚀
