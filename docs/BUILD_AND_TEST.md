# Build and Test Guide

## Quick Start - Testing Without Hardware

### Step 1: Build the Test Program

```bash
cd /Users/vapourism/Desktop/code/ImuCamera
mkdir -p build
cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_WITH_MOCK_HARDWARE=ON
make test_hardware_mock
```

### Step 2: Run the Test Suite

```bash
./tests/test_hardware_mock
```

This will:
- ✅ Generate fake IMU sensor data
- ✅ Simulate camera control operations
- ✅ Test error handling
- ✅ Coordinate IMU + Camera together
- ✅ Generate `test_hardware.log` with detailed logs

### Expected Output

```
╔════════════════════════════════════════════════════════════╗
║         ImuCamera Hardware Mock Test Suite                 ║
╚════════════════════════════════════════════════════════════╝

┌────────────────────────────────────────────────────────────┐
│ Test 1: IMU Reader (Static Mode)                           │
└────────────────────────────────────────────────────────────┘
Reading IMU data (static mode)...
  Sample 0: pitch=0.00° yaw=0.00° roll=0.00°
  Sample 1: pitch=0.00° yaw=0.00° roll=0.00°
  ...
✓ Static mode test passed

[... more tests ...]

All hardware tests completed successfully\!
```

## Testing Components

### 1. MockIMUReader - Simulates IMU Sensor Data

**Simulation Modes:**
```cpp
MockIMUReader::STATIC      // No motion
MockIMUReader::SINE_WAVE   // Smooth oscillation (most realistic)
MockIMUReader::RANDOM      // Random values
MockIMUReader::CIRCLE      // Circular motion
MockIMUReader::STEP_CHANGES // Sudden movements
```

**Usage:**
```cpp
MockIMUReader imu("/dev/null", 115200, MockIMUReader::SINE_WAVE);
imu.setSimulationParams(45.0f, 45.0f, 1.0f);  // amplitude, amplitude, frequency
imu.setNoiseLevel(0.5f);

imu.start();
IMUData data;
while (imu.waitForData(data, 1000)) {
    std::cout << "Pitch: " << data.pitch << "°\n";
}
imu.stop();
```

### 2. MockCameraControl - Simulates PTZ Camera

**Supported Operations:**
```cpp
camera.connect();                    // Mock connection
camera.pan(speed, direction);        // Rotate left/right
camera.tilt(speed, direction);       // Rotate up/down
camera.zoom(speed, direction);       // Zoom in/out
camera.setPosition(pan, tilt);       // Absolute positioning
camera.captureFrame(path);           // Save frame (no actual capture)
camera.disconnect();
```

**Features:**
- Operation history logging (for verification)
- Simulated position tracking
- Error simulation capability

## Build Configurations

### Configuration 1: Test with Mock Hardware (Recommended)

```bash
cmake .. -DBUILD_TESTS=ON -DBUILD_WITH_MOCK_HARDWARE=ON
make
./tests/test_hardware_mock
```

**Pros:**
- No real hardware needed
- Fast testing
- Reproducible results
- Good for CI/CD

### Configuration 2: Production Build (When Hardware Available)

```bash
cmake .. -DBUILD_TESTS=OFF -DBUILD_WITH_MOCK_HARDWARE=OFF
make imu_camera_new
./imu_camera_new ../config.ini
```

**Pros:**
- Real hardware integration
- Production-ready

### Configuration 3: Both Build Targets

```bash
cmake .. -DBUILD_TESTS=ON -DBUILD_WITH_MOCK_HARDWARE=OFF
make
./tests/test_hardware_mock    # Use mocks
./imu_camera_new config.ini   # Use real hardware
```

## CMake Build Options

```bash
# Option 1: Build with mock hardware for testing
-DBUILD_WITH_MOCK_HARDWARE=ON

# Option 2: Build test executables
-DBUILD_TESTS=ON

# Option 3: Generate documentation
-DBUILD_DOCS=ON

# Example:
cmake .. -DBUILD_TESTS=ON -DBUILD_WITH_MOCK_HARDWARE=ON -DBUILD_DOCS=ON
```

## Running Individual Tests

If you want to run tests programmatically with different parameters:

```cpp
// Create your own test file (e.g., my_test.cpp)
#include "src/hardware/MockIMUReader.h"
#include "src/hardware/MockCameraControl.h"

int main() {
    // Test 1: IMU static data
    {
        MockIMUReader imu("/dev/null", 115200, MockIMUReader::STATIC);
        imu.start();
        
        IMUData data;
        for (int i = 0; i < 100; i++) {
            imu.tryGetData(data);
            assert(data.pitch == 0.0f);
        }
        
        imu.stop();
        std::cout << "✓ Static test passed\n";
    }
    
    // Test 2: Camera operations
    {
        MockCameraControl camera("192.168.1.8", 8000, "admin", "password");
        assert(camera.connect());
        assert(camera.pan(64, 1));
        assert(camera.tilt(32, 0));
        assert(camera.zoom(64, 1));
        assert(camera.disconnect());
        
        std::cout << "✓ Camera test passed\n";
    }
    
    return 0;
}
```

Compile with:
```bash
g++ -std=c++14 -I. my_test.cpp src/hardware/MockIMUReader.cpp src/hardware/MockCameraControl.cpp src/utils/Logger.cpp -pthread -o my_test
./my_test
```

## Continuous Testing (CTest)

After building:

```bash
cd build
ctest              # Run all tests
ctest --verbose    # Run with detailed output
ctest -R Hardware  # Run only hardware tests
```

## Debugging Tests

Enable detailed logging:

```cpp
// In your test
Logger::getInstance().init("test_debug.log", Logger::Level::DEBUG);

// Then run test and check test_debug.log for all operations
```

Check logs:
```bash
tail -f test_hardware.log
tail -f test_debug.log
```

## Integration Testing

Test the complete system (IMU + Camera + Application):

```cpp
// Pseudo-code for integration test
MockIMUReader imu(..., MockIMUReader::SINE_WAVE);
MockCameraControl camera(...);

imu.start();
camera.connect();

// Simulate coordination
for (int i = 0; i < 1000; i++) {
    IMUData data;
    if (imu.waitForData(data, 100)) {
        // Map IMU to camera position
        int pan = (int)(data.yaw * 10);
        int tilt = (int)(data.pitch * 10);
        camera.setPosition(pan, tilt);
        
        // Verify position
        assert(camera.getPanPosition() == pan);
        assert(camera.getTiltPosition() == tilt);
    }
}

imu.stop();
camera.disconnect();
```

## Troubleshooting Build Issues

### Issue: "OpenCV not found"
```bash
# Install OpenCV
brew install opencv  # macOS
sudo apt-get install libopencv-dev  # Linux

# Or specify path
cmake .. -DOpenCV_DIR=/path/to/opencv/build
```

### Issue: "pthread not found"
```bash
# Usually included by default on Linux/macOS
# If not, install:
sudo apt-get install libpthread-stubs0-dev  # Linux
```

### Issue: "cannot find source files"
```bash
# Verify you're in the correct directory
cd /Users/vapourism/Desktop/code/ImuCamera
pwd  # Should show the project directory
```

### Issue: "test_hardware_mock not created"
```bash
# Check CMakeLists.txt has correct paths
# Verify files exist:
ls -la src/hardware/MockIMUReader.cpp
ls -la src/hardware/MockCameraControl.cpp
ls -la test_hardware_main.cpp

# Rebuild
rm -rf build
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
make
```

## Performance Testing

Measure mock data generation rate:

```cpp
auto start = std::chrono::high_resolution_clock::now();

MockIMUReader imu(...);
imu.start();

for (int i = 0; i < 10000; i++) {
    IMUData data;
    imu.waitForData(data);
}

auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::high_resolution_clock::now() - start
);

std::cout << "Generated 10000 samples in " << elapsed.count() << "ms\n";
std::cout << "Rate: " << (10000.0 / elapsed.count() * 1000) << " samples/sec\n";
```

Expected: ~100 Hz sampling rate

## Next Steps

1. ✅ Run `make test_hardware_mock`
2. ✅ Run `./tests/test_hardware_mock`
3. ✅ Review test output and logs
4. ✅ Read [TESTING_WITHOUT_HARDWARE.md](TESTING_WITHOUT_HARDWARE.md) for detailed testing guide
5. ✅ When hardware available, switch to real implementation

## Related Documentation

- [TESTING_WITHOUT_HARDWARE.md](TESTING_WITHOUT_HARDWARE.md) - Detailed mock testing guide
- [HARDWARE_MODULE_DESIGN.md](HARDWARE_MODULE_DESIGN.md) - Hardware architecture
- [QUICKSTART.md](QUICKSTART.md) - General project quick start
- [test_hardware_main.cpp](test_hardware_main.cpp) - Full test source code

---

**Happy Testing\!** 🧪
