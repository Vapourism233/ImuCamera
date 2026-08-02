#include "MockIMUReader.h"
#include <random>
#include <thread>
#include <chrono>
#include <cmath>

namespace hardware {

MockIMUReader::MockIMUReader(const std::string& device, 
                             uint32_t baudrate,
                             SimulationMode mode)
    : mode_(mode), device_(device), baudrate_(baudrate) {
    utils::Logger::getInstance().info(
        "MockIMUReader created with mode: " + std::to_string(mode));
}

MockIMUReader::~MockIMUReader() {
    stop();
    utils::Logger::getInstance().info("MockIMUReader destroyed");
}

bool MockIMUReader::start() {
    if (running_) {
        utils::Logger::getInstance().warning("MockIMUReader already running");
        return true;
    }
    
    running_ = true;
    sample_count_ = 0;
    reader_thread_ = std::thread(&MockIMUReader::readerThreadFunc, this);
    
    utils::Logger::getInstance().info("MockIMUReader started");
    return true;
}

bool MockIMUReader::stop() {
    if (!running_) {
        return true;
    }
    
    running_ = false;
    if (reader_thread_.joinable()) {
        reader_thread_.join();
    }
    
    utils::Logger::getInstance().info(
        "MockIMUReader stopped. Total samples: " + std::to_string(sample_count_));
    return true;
}

bool MockIMUReader::tryGetData(IMUData& data) {
    return data_queue_.tryPop(data);
}

bool MockIMUReader::waitForData(IMUData& data, uint32_t timeout_ms) {
    if (timeout_ms == 0) {
        data_queue_.waitAndPop(data);
        return true;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        if (data_queue_.tryPop(data)) {
            return true;
        }
        
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > timeout_ms) {
            return false;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void MockIMUReader::setSimulationParams(float pitch_amplitude,
                                       float yaw_amplitude,
                                       float frequency) {
    pitch_amplitude_ = pitch_amplitude;
    yaw_amplitude_ = yaw_amplitude;
    frequency_ = frequency;
}

void MockIMUReader::setNoiseLevel(float noise_level) {
    noise_level_ = noise_level;
}

void MockIMUReader::readerThreadFunc() {
    utils::Logger::getInstance().debug("IMU reader thread started");
    
    while (running_) {
        IMUData data;
        
        // Generate data based on mode
        switch (mode_) {
            case STATIC:
                data = generateStaticData();
                break;
            case SINE_WAVE:
                data = generateSineWaveData(sample_count_);
                break;
            case RANDOM:
                data = generateRandomData();
                break;
            case CIRCLE:
                data = generateCircleData(sample_count_);
                break;
            case STEP_CHANGES:
                data = generateStepChangeData(sample_count_);
                break;
        }
        
        data_queue_.push(data);
        sample_count_++;
        
        // Simulate ~100 Hz sampling rate
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    utils::Logger::getInstance().debug("IMU reader thread terminated");
}

IMUData MockIMUReader::generateStaticData() {
    IMUData data;
    data.pitch = 0.0f;
    data.yaw = 0.0f;
    data.roll = 0.0f;
    data.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    return data;
}

IMUData MockIMUReader::generateSineWaveData(uint64_t sample_num) {
    IMUData data;
    
    float time_s = sample_num * 0.01f; // 10ms per sample
    float angle = 2.0f * M_PI * frequency_ * time_s;
    
    data.pitch = pitch_amplitude_ * std::sin(angle);
    data.yaw = yaw_amplitude_ * std::sin(angle + M_PI / 4.0f);  // 45 degree offset
    data.roll = (pitch_amplitude_ * 0.3f) * std::sin(angle + M_PI / 2.0f);
    
    // Add noise
    data.pitch = addNoise(data.pitch);
    data.yaw = addNoise(data.yaw);
    data.roll = addNoise(data.roll);
    
    data.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    
    return data;
}

IMUData MockIMUReader::generateRandomData() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    
    IMUData data;
    data.pitch = pitch_amplitude_ * dis(gen);
    data.yaw = yaw_amplitude_ * dis(gen);
    data.roll = (pitch_amplitude_ * 0.3f) * dis(gen);
    
    data.pitch = addNoise(data.pitch);
    data.yaw = addNoise(data.yaw);
    data.roll = addNoise(data.roll);
    
    data.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    
    return data;
}

IMUData MockIMUReader::generateCircleData(uint64_t sample_num) {
    IMUData data;
    
    float time_s = sample_num * 0.01f;
    float angle = 2.0f * M_PI * frequency_ * time_s;
    
    data.pitch = pitch_amplitude_ * std::sin(angle);
    data.yaw = yaw_amplitude_ * std::cos(angle);
    data.roll = 0.0f;
    
    data.pitch = addNoise(data.pitch);
    data.yaw = addNoise(data.yaw);
    data.roll = addNoise(data.roll);
    
    data.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    
    return data;
}

IMUData MockIMUReader::generateStepChangeData(uint64_t sample_num) {
    IMUData data;
    
    // Change values every 100 samples (~1 second)
    uint64_t step = sample_num / 100;
    float multiplier = (step % 2 == 0) ? 1.0f : -1.0f;
    
    data.pitch = pitch_amplitude_ * multiplier;
    data.yaw = yaw_amplitude_ * multiplier;
    data.roll = 0.0f;
    
    data.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    
    return data;
}

float MockIMUReader::addNoise(float value) {
    if (noise_level_ < 0.001f) return value;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<> dis(0.0, noise_level_);
    
    return value + dis(gen);
}

} // namespace hardware
