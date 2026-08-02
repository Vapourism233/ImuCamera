# Hardware Testing Guide - No Physical Hardware Required

## Overview

This project includes comprehensive mock implementations of all hardware components, allowing you to test the entire system without physical IMU sensors or cameras.

## Why Mock Testing?

✅ **No Hardware Required** - Develop and test on any machine
✅ **Fast Iteration** - No wait for slow serial communication
✅ **Reproducible Tests** - Same data every time (or controlled randomness)
✅ **Error Simulation** - Test error handling without breaking hardware
✅ **CI/CD Ready** - Run tests in automated pipelines
✅ **Cost Effective** - No need to risk hardware during development

## Available Mock Components

### 1. MockIMUReader - Simulates IMU Sensor

**Location:** `src/hardware/MockIMUReader.h/cpp`

**Features:**
- Multiple simulation modes (Static, Sine Wave, Random, Circle, Step Changes)
- Configurable amplitude and frequency
- Realistic noise simulation
- Thread-safe operation (same API as real IMUReader)

**Modes:**

| Mode | Use Case | Description |
|------|----------|-------------|
| **STATIC** | Testing with no motion | All sensor values = 0 |
| **SINE_WAVE** | Realistic oscillation | Smooth periodic motion (most realistic) |
| **RANDOM** | Unpredictable motion | Random variations in all axes |
| **CIRCLE** | Circular motion | Pan & yaw trace a circle |
| **STEP_CHANGES** | Sudden movements | Abrupt changes in position |

### 2. MockCameraControl - Simulates PTZ Camera

**Location:** `src/hardware/MockCameraControl.h/cpp`

**Features:**
- Full pan/tilt/zoom control simulation
- Realistic position constraints
- Operation history logging (for verification)
- Error simulation capability
- No network latency

## Running Tests

### Option 1: Simple Test Program (Recommended for Quick Testing)

```bash
cd /Users/vapourism/Desktop/code/ImuCamera
./test_hardware_mock
```

This will run a comprehensive test suite showing:
- Static IMU data
- Sine wave motion
- Random motion
- Camera control operations
- Error handling
- Integration test

**Output:**
- Console output with test results
- `test_hardware.log` with detailed logs

### Option 2: Individual Component Testing

Create your own test file and link against the mocks:

```cpp
#include "src/hardware/MockIMUReader.h"
#include "src/hardware/MockCameraControl.h"

int main() {
    // Test IMU in sine wave mode
    MockIMUReader imu("/dev/null", 115200, MockIMUReader::SINE_WAVE);
    imu.setSimulationParams(45.0f, 45.0f, 1.0f);  // 45° amplitude, 1 Hz
    imu.start();
    
    IMUData data;
    if (imu.waitForData(data, 1000)) {
        // Process data
    }
    
    imu.stop();
    return 0;
}
```

## Test Scenarios

### Scenario 1: Basic Hardware Initialization

```cpp
MockIMUReader imu("/dev/null", 115200);
MockCameraControl camera("192.168.1.8", 8000, "admin", "password");

// Initialize
imu.start();
camera.connect();

// Verify
assert(imu started successfully);
assert(camera.isConnected());

// Cleanup
imu.stop();
camera.disconnect();
```

### Scenario 2: Continuous Motion Tracking

```cpp
MockIMUReader imu("/dev/null", 115200, MockIMUReader::SINE_WAVE);
imu.setSimulationParams(30.0f, 30.0f, 0.5f);
imu.start();

MockCameraControl camera("192.168.1.8");
camera.connect();

for (int i = 0; i < 100; i++) {
    IMUData data;
    if (imu.waitForData(data, 200)) {
        // Map IMU data to camera position
        camera.setPosition((int32_t)(data.yaw * 10), 
                          (int32_t)(data.pitch * 10));
    }
}

imu.stop();
camera.disconnect();
```

### Scenario 3: Error Handling

```cpp
MockCameraControl camera("192.168.1.8");
camera.connect();

// Simulate connection failure
camera.simulateError(true);
bool success = camera.pan(64, 1);
assert(!success);  // Should fail

// Normal operation
success = camera.pan(64, 1);
assert(success);   // Should succeed
```

### Scenario 4: Operation Verification

```cpp
MockCameraControl camera("192.168.1.8");
camera.connect();

// Perform operations
camera.pan(100, 1);
camera.tilt(50, 0);
camera.zoom(75, 1);

// Verify history
const auto& history = camera.getOperationHistory();
for (const auto& op : history) {
    std::cout << "Operation: " << op.operation << "\n";
    std::cout << "Parameters: " << op.parameters << "\n";
}
```

## Build Configuration

### CMakeLists.txt Setup

The `CMakeLists.txt` should include:

```cmake
# Mock implementations (for testing without hardware)
set(MOCK_SOURCES
    src/hardware/MockIMUReader.cpp
    src/hardware/MockCameraControl.cpp
)

# Test executable
add_executable(test_hardware_mock
    test_hardware_main.cpp
    src/utils/Logger.cpp
    src/utils/ThreadSafeQueue.cpp  # Header-only but may need implementation
    ${MOCK_SOURCES}
)

target_include_directories(test_hardware_mock PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_hardware_mock pthread)
```

## Advanced Testing Strategies

### Testing Hardware Manager with Mocks

```cpp
#include "src/hardware/HardwareManager.h"
#include "src/hardware/MockIMUReader.h"
#include "src/hardware/MockCameraControl.h"

// Option 1: Compile with mock implementations instead of real ones
// Option 2: Dependency injection (refactor HardwareManager to accept mock factories)

HardwareManager::Config config;
config.imu_device = "/dev/null";
config.camera_ip = "192.168.1.8";

HardwareManager& mgr = HardwareManager::getInstance();
if (mgr.init(config)) {
    IMUData data;
    if (mgr.getLatestIMUData(data)) {
        // Process data without real hardware
    }
}
```

### Performance Testing

```cpp
MockIMUReader imu("/dev/null", 115200, MockIMUReader::SINE_WAVE);
imu.start();

auto start = std::chrono::high_resolution_clock::now();

uint64_t samples = 0;
IMUData data;
while (samples < 10000) {  // Collect 10,000 samples (~100 seconds at 100 Hz)
    if (imu.waitForData(data, 500)) {
        samples++;
    }
}

auto elapsed = std::chrono::high_resolution_clock::now() - start;
auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

std::cout << "Processed " << samples << " samples in " << ms << "ms\n";
std::cout << "Throughput: " << (1000.0 * samples / ms) << " samples/sec\n";
```

### Stress Testing

```cpp
// Test with very high frequency
MockIMUReader imu("/dev/null", 115200, MockIMUReader::RANDOM);
imu.setSimulationParams(89.9f, 89.9f, 10.0f);  // High frequency
imu.setNoiseLevel(5.0f);  // High noise
imu.start();

// Rapidly consume data
for (int i = 0; i < 1000; i++) {
    IMUData data;
    if (imu.tryGetData(data)) {
        // Verify data is valid
        assert(data.pitch <= 90.0f && data.pitch >= -90.0f);
    }
}

imu.stop();
```

## Switching Between Mock and Real Hardware

### Method 1: Compile-Time Selection

```cpp
#ifdef USE_MOCK_HARDWARE
    #include "src/hardware/MockIMUReader.h"
    using IMUReaderImpl = MockIMUReader;
#else
    #include "src/hardware/IMUReader.h"
    using IMUReaderImpl = IMUReader;
#endif

// Same code works for both
IMUReaderImpl imu("/dev/ttyUSB0", 115200);
```

### Method 2: Runtime Selection

```cpp
class IMUReaderFactory {
public:
    static std::unique_ptr<IMUReaderInterface> create(bool use_mock) {
        if (use_mock) {
            return std::make_unique<MockIMUReader>();
        } else {
            return std::make_unique<IMUReader>();
        }
    }
};
```

## Mock Data Characteristics

### Sine Wave Mode (Most Realistic)

```
Parameter: 30° amplitude, 1 Hz frequency
Expected Output:
Time(s) | Pitch(°) | Yaw(°)  | Roll(°)
--------|----------|---------|----------
0.0     | 0.0      | 30.0    | 0.0
0.25    | 30.0     | 0.0     | -30.0
0.50    | 0.0      | -30.0   | 0.0
0.75    | -30.0    | 0.0     | 30.0
1.00    | 0.0      | 30.0    | 0.0
```

### Random Mode (Unpredictable)

```
Each sample: random value in [-amplitude, +amplitude]
Good for testing edge cases and stress conditions
```

## Debugging Mock Operations

Enable detailed logging:

```cpp
Logger::getInstance().init("debug.log", Logger::DEBUG);

MockIMUReader imu("/dev/null");
imu.start();

// All operations logged to file and console
// Check debug.log for detailed trace
```

## Common Issues & Solutions

**Issue: Mock data not being generated**
```cpp
// Solution: Ensure you called start()
imu.start();  // Important!
std::this_thread::sleep_for(std::chrono::milliseconds(20));  // Wait for first sample
```

**Issue: Queue timeout waiting for data**
```cpp
// Solution: Increase timeout or add delay
IMUData data;
imu.waitForData(data, 1000);  // 1 second timeout
// or
std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Wait for generation
imu.tryGetData(data);
```

**Issue: Camera position not changing**
```cpp
// Solution: Remember mock position is relative, not absolute
// Each pan/tilt command updates position based on speed/direction
for (int i = 0; i < 10; i++) {
    camera.pan(64, 1);  // Move right each iteration
}
```

## Next Steps

1. ✅ Run the test suite: `./test_hardware_mock`
2. ✅ Review operation logs in `test_hardware.log`
3. ✅ Create custom test scenarios for your use case
4. ✅ Integrate mocks into your build system
5. ✅ Set up CI/CD pipeline with mock tests
6. ✅ When hardware is available, replace mocks with real implementations

## Performance Expectations

With mock implementations (no real hardware):
- **IMU Data Rate**: ~100 Hz (configurable)
- **Latency**: < 1ms per sample
- **Camera Response**: Instant (no network latency)
- **Memory**: < 5 MB for test program
- **CPU**: < 5% for both components running

## See Also

- [QUICKSTART.md](QUICKSTART.md) - General project quick start
- [HARDWARE_MODULE_DESIGN.md](HARDWARE_MODULE_DESIGN.md) - Architecture details
- [test_hardware_main.cpp](test_hardware_main.cpp) - Test program source code

---

**Ready to test without hardware!** 🚀

Just build and run `test_hardware_mock` to verify everything works.
