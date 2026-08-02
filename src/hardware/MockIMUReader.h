#ifndef MOCK_IMU_READER_H
#define MOCK_IMU_READER_H

#include <string>
#include <atomic>
#include <chrono>
#include <cmath>
#include <thread>
#include "../utils/Logger.h"
#include "../utils/ThreadSafeQueue.h"
#include "IMUData.h"

namespace hardware {

/**
 * @brief Mock IMU Reader - simulates IMU sensor behavior without hardware
 * 
 * This class provides simulated IMU data for testing and development without
 * requiring actual hardware. It can simulate:
 * - Realistic sensor data with sine-wave motion
 * - Various motion patterns (rotations, random variations)
 * - Sensor errors and noise
 * 
 * Usage exactly same as IMUReader - just use MockIMUReader instead
 */
class MockIMUReader {
public:
    enum SimulationMode {
        STATIC,        // All values constant (no motion)
        SINE_WAVE,     // Smooth sine wave motion (realistic)
        RANDOM,        // Random variations
        CIRCLE,        // Circular motion pattern
        STEP_CHANGES   // Step changes (sudden movements)
    };

    /**
     * Constructor
     * @param device Ignored (for API compatibility)
     * @param baudrate Ignored (for API compatibility)
     * @param mode Simulation mode (default: SINE_WAVE)
     */
    MockIMUReader(const std::string& device = "/dev/null", 
                  uint32_t baudrate = 115200,
                  SimulationMode mode = SINE_WAVE);
    
    /**
     * Destructor
     */
    ~MockIMUReader();

    /**
     * Start the mock IMU reader thread
     * @return true (always succeeds)
     */
    bool start();

    /**
     * Stop the mock IMU reader thread
     * @return true (always succeeds)
     */
    bool stop();

    /**
     * Try to get IMU data (non-blocking)
     * @param data Output IMU data
     * @return true if data available, false if queue empty
     */
    bool tryGetData(IMUData& data);

    /**
     * Wait and get IMU data (blocking)
     * @param data Output IMU data
     * @param timeout_ms Timeout in milliseconds (0 = infinite)
     * @return true if data received, false on timeout
     */
    bool waitForData(IMUData& data, uint32_t timeout_ms = 0);

    /**
     * Set simulation parameters
     * @param pitch_amplitude Pitch rotation amplitude in degrees
     * @param yaw_amplitude Yaw rotation amplitude in degrees
     * @param frequency Oscillation frequency in Hz
     */
    void setSimulationParams(float pitch_amplitude = 30.0f,
                            float yaw_amplitude = 30.0f,
                            float frequency = 0.5f);

    /**
     * Inject noise into sensor readings
     * @param noise_level Noise standard deviation (0.0 = no noise)
     */
    void setNoiseLevel(float noise_level = 0.5f);

    /**
     * Get current simulation mode
     */
    SimulationMode getMode() const { return mode_; }

    /**
     * Set simulation mode
     */
    void setMode(SimulationMode mode) { mode_ = mode; }

    /**
     * Get total samples generated (for testing)
     */
    uint64_t getSampleCount() const { return sample_count_; }

private:
    SimulationMode mode_;
    std::string device_;
    uint32_t baudrate_;
    
    std::atomic<bool> running_{false};
    std::thread reader_thread_;
    utils::ThreadSafeQueue<IMUData> data_queue_;
    
    // Simulation parameters
    float pitch_amplitude_{30.0f};
    float yaw_amplitude_{30.0f};
    float frequency_{0.5f};
    float noise_level_{0.5f};
    std::atomic<uint64_t> sample_count_{0};
    
    // Helper methods
    void readerThreadFunc();
    IMUData generateStaticData();
    IMUData generateSineWaveData(uint64_t sample_num);
    IMUData generateRandomData();
    IMUData generateCircleData(uint64_t sample_num);
    IMUData generateStepChangeData(uint64_t sample_num);
    float addNoise(float value);
};

} // namespace hardware

#endif // MOCK_IMU_READER_H
