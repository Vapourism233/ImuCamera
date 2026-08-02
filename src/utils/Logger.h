#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <memory>
#include <mutex>

namespace utils {

/**
 * @brief Simple thread-safe logger utility
 * Supports file and console logging with different severity levels
 */
class Logger {
public:
    enum class Level {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    /**
     * Initialize logger with file path
     * @param filepath Path to log file. If empty, only console logging.
     * @param level Minimum severity level to log
     */
    void init(const std::string& filepath = "", Level level = Level::INFO) {
        std::lock_guard<std::mutex> lock(mutex_);
        min_level_ = level;
        
        if (!filepath.empty()) {
            log_file_.open(filepath, std::ios::app);
            if (!log_file_.is_open()) {
                std::cerr << "Failed to open log file: " << filepath << std::endl;
            }
        }
    }

    void debug(const std::string& message) { log(Level::DEBUG, message); }
    void info(const std::string& message) { log(Level::INFO, message); }
    void warning(const std::string& message) { log(Level::WARNING, message); }
    void error(const std::string& message) { log(Level::ERROR, message); }
    void critical(const std::string& message) { log(Level::CRITICAL, message); }

    // Convenience methods with formatting
    template<typename... Args>
    void debugf(const char* format, Args... args) {
        logf(Level::DEBUG, format, args...);
    }

    template<typename... Args>
    void infof(const char* format, Args... args) {
        logf(Level::INFO, format, args...);
    }

    template<typename... Args>
    void warningf(const char* format, Args... args) {
        logf(Level::WARNING, format, args...);
    }

    template<typename... Args>
    void errorf(const char* format, Args... args) {
        logf(Level::ERROR, format, args...);
    }

    template<typename... Args>
    void criticalf(const char* format, Args... args) {
        logf(Level::CRITICAL, format, args...);
    }

    ~Logger() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(Level level, const std::string& message) {
        if (static_cast<int>(level) < static_cast<int>(min_level_)) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        std::string level_str = levelToString(level);
        std::string timestamp = getCurrentTimestamp();
        std::string log_msg = "[" + timestamp + "] [" + level_str + "] " + message;

        // Console output
        std::cout << log_msg << std::endl;

        // File output
        if (log_file_.is_open()) {
            log_file_ << log_msg << std::endl;
            log_file_.flush();
        }
    }

    template<typename... Args>
    void logf(Level level, const char* format, Args... args) {
        // Simple printf-style formatting
        char buffer[4096];
        snprintf(buffer, sizeof(buffer), format, args...);
        log(level, std::string(buffer));
    }

    static std::string levelToString(Level level) {
        switch (level) {
            case Level::DEBUG:    return "DEBUG";
            case Level::INFO:     return "INFO";
            case Level::WARNING:  return "WARNING";
            case Level::ERROR:    return "ERROR";
            case Level::CRITICAL: return "CRITICAL";
            default:              return "UNKNOWN";
        }
    }

    static std::string getCurrentTimestamp() {
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return std::string(buffer);
    }

    std::ofstream log_file_;
    Level min_level_;
    std::mutex mutex_;
};

}  // namespace utils

#endif  // LOGGER_H
