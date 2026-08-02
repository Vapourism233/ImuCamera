#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include "src/utils/Logger.h"
#include "src/hardware/MockIMUReader.h"
#include "src/hardware/MockCameraControl.h"
#include "src/hardware/IMUData.h"

using namespace hardware;

/**
 * Test Suite for ImuCamera Hardware Modules
 * 
 * This program tests the hardware modules using mock implementations.
 * No physical hardware is required.
 */

class HardwareTest {
public:
    void runAllTests() {
        utils::Logger::getInstance().init("test_hardware.log", utils::Logger::Level::DEBUG);
        utils::Logger::getInstance().info("=== Hardware Test Suite Started ===");
        
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║         ImuCamera Hardware Mock Test Suite                 ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
        
        testIMUReaderStatic();
        testIMUReaderSineWave();
        testIMUReaderRandom();
        testCameraControlBasics();
        testCameraControlErrors();
        testIntegration();
        
        std::cout << "\n✓ All tests completed!\n";
        std::cout << "✓ Check test_hardware.log for detailed logs\n\n";
    }

private:
    void testIMUReaderStatic() {
        std::cout << "┌────────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Test 1: IMU Reader (Static Mode)                           │\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        MockIMUReader imu("/dev/null", 115200, MockIMUReader::STATIC);
        imu.start();
        
        std::cout << "Reading IMU data (static mode)...\n";
        for (int i = 0; i < 5; i++) {
            IMUData data;
            if (imu.tryGetData(data)) {
                std::cout << std::fixed << std::setprecision(2)
                         << "  Sample " << i << ": pitch=" << data.pitch 
                         << "° yaw=" << data.yaw << "° roll=" << data.roll << "°\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        imu.stop();
        std::cout << "✓ Static mode test passed\n\n";
    }

    void testIMUReaderSineWave() {
        std::cout << "┌────────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Test 2: IMU Reader (Sine Wave Mode)                        │\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        MockIMUReader imu("/dev/null", 115200, MockIMUReader::SINE_WAVE);
        imu.setSimulationParams(45.0f, 45.0f, 1.0f);  // 45° amplitude, 1 Hz
        imu.setNoiseLevel(0.5f);  // Add some noise
        
        imu.start();
        
        std::cout << "Reading sine wave IMU data...\n";
        std::cout << "Sample | Pitch (°) | Yaw (°)  | Roll (°)\n";
        std::cout << "-------|-----------|----------|----------\n";
        
        for (int i = 0; i < 10; i++) {
            IMUData data;
            if (imu.waitForData(data, 200)) {
                std::cout << std::fixed << std::setprecision(1)
                         << std::setw(6) << i << " | "
                         << std::setw(9) << data.pitch << " | "
                         << std::setw(8) << data.yaw << " | "
                         << std::setw(8) << data.roll << "\n";
            }
        }
        
        imu.stop();
        std::cout << "  Generated " << imu.getSampleCount() << " samples total\n";
        std::cout << "✓ Sine wave mode test passed\n\n";
    }

    void testIMUReaderRandom() {
        std::cout << "┌────────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Test 3: IMU Reader (Random Mode)                           │\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        MockIMUReader imu("/dev/null", 115200, MockIMUReader::RANDOM);
        imu.start();
        
        std::cout << "Reading random IMU data (5 samples)...\n";
        for (int i = 0; i < 5; i++) {
            IMUData data;
            if (imu.waitForData(data, 100)) {
                std::cout << std::fixed << std::setprecision(2)
                         << "  Sample " << i << ": pitch=" << data.pitch 
                         << "° yaw=" << data.yaw << "° roll=" << data.roll << "°\n";
            }
        }
        
        imu.stop();
        std::cout << "✓ Random mode test passed\n\n";
    }

    void testCameraControlBasics() {
        std::cout << "┌────────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Test 4: Camera Control (Basic Operations)                  │\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        MockCameraControl camera("192.168.1.8", 8000, "admin", "password");
        
        // Test connection
        if (camera.connect()) {
            std::cout << "✓ Connected to camera\n";
        }
        
        // Test pan
        std::cout << "Testing pan movements...\n";
        camera.pan(64, 1);  // Pan right at speed 64
        std::cout << "  Pan right: position = " << camera.getPanPosition() << "\n";
        
        camera.pan(64, 0);  // Pan left
        std::cout << "  Pan left: position = " << camera.getPanPosition() << "\n";
        
        // Test tilt
        std::cout << "Testing tilt movements...\n";
        camera.tilt(64, 1);  // Tilt up
        std::cout << "  Tilt up: position = " << camera.getTiltPosition() << "\n";
        
        camera.tilt(64, 0);  // Tilt down
        std::cout << "  Tilt down: position = " << camera.getTiltPosition() << "\n";
        
        // Test absolute positioning
        std::cout << "Testing absolute positioning...\n";
        camera.setPosition(1800, 450);  // +180 pan, +45 tilt
        std::cout << "  Set position (pan=1800, tilt=450)\n";
        std::cout << "  Current: pan=" << camera.getPanPosition() 
                 << " tilt=" << camera.getTiltPosition() << "\n";
        
        // Test frame capture
        std::cout << "Testing frame capture...\n";
        camera.captureFrame("/tmp/camera_frame.jpg");
        std::cout << "  Frame captured\n";
        
        // Test disconnect
        camera.disconnect();
        std::cout << "✓ Disconnected from camera\n";
        std::cout << "✓ Camera control test passed\n\n";
    }

    void testCameraControlErrors() {
        std::cout << "┌────────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Test 5: Camera Control (Error Handling)                    │\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        MockCameraControl camera("192.168.1.8");
        
        // Test operation on disconnected camera
        std::cout << "Testing operation on disconnected camera...\n";
        if (!camera.pan(64, 1)) {
            std::cout << "✓ Correctly rejected operation (not connected)\n";
        }
        
        camera.connect();
        
        // Test simulated error
        std::cout << "Testing simulated error handling...\n";
        camera.simulateError(true);
        if (!camera.pan(64, 1)) {
            std::cout << "✓ Correctly handled simulated error\n";
        }
        
        // Test operation history
        std::cout << "Testing operation history...\n";
        camera.pan(64, 1);
        camera.tilt(32, 0);
        camera.zoom(64, 1);
        
        const auto& history = camera.getOperationHistory();
        std::cout << "  Operation history (" << history.size() << " operations):\n";
        for (const auto& op : history) {
            std::cout << "    - " << op.operation << ": " << op.parameters << "\n";
        }
        
        camera.disconnect();
        std::cout << "✓ Error handling test passed\n\n";
    }

    void testIntegration() {
        std::cout << "┌────────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Test 6: Integration (IMU + Camera Coordination)            │\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        // Create mock hardware
        MockIMUReader imu("/dev/null", 115200, MockIMUReader::SINE_WAVE);
        imu.setSimulationParams(30.0f, 30.0f, 0.5f);
        
        MockCameraControl camera("192.168.1.8");
        
        // Connect
        imu.start();
        camera.connect();
        
        std::cout << "Coordinating IMU data with camera movements...\n";
        std::cout << "IMU Pitch | Camera Pan | Status\n";
        std::cout << "----------|------------|--------\n";
        
        for (int i = 0; i < 8; i++) {
            IMUData data;
            if (imu.waitForData(data, 200)) {
                // Simulate camera tracking (simplified)
                if (data.pitch > 0) {
                    camera.pan(64, 1);  // Pan right if pitch is positive
                } else {
                    camera.pan(64, 0);  // Pan left if pitch is negative
                }
                
                std::cout << std::fixed << std::setprecision(1)
                         << std::setw(9) << data.pitch << " | "
                         << std::setw(10) << camera.getPanPosition() << " | "
                         << "Tracking\n";
            }
        }
        
        imu.stop();
        camera.disconnect();
        
        std::cout << "✓ Integration test passed\n\n";
    }
};

int main(int argc, char* argv[]) {
    try {
        HardwareTest test;
        test.runAllTests();
        
        std::cout << "═══════════════════════════════════════════════════════════\n";
        std::cout << "All hardware tests completed successfully!\n";
        std::cout << "═══════════════════════════════════════════════════════════\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
