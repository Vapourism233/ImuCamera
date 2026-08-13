cmake_minimum_required(VERSION 3.16)
project(ImuCamera)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# Configuration Options
# ============================================================================

option(BUILD_WITH_MOCK_HARDWARE "Build with mock hardware for testing" OFF)
option(BUILD_TESTS "Build test programs" ON)
option(BUILD_PRODUCTION_APP "Build production application (imu_camera_new)" OFF)
option(BUILD_DOCS "Generate documentation" OFF)

# ============================================================================
# Find Dependencies
# ============================================================================

# OpenCV is only needed for production build (vision processing)
# For testing with mocks, it's not required
find_package(OpenCV QUIET COMPONENTS core)
find_package(Threads REQUIRED)

# Optional: HIKvision SDK (not required for mock testing)
# set(HIKVISION_SDK_PATH "/path/to/hikvision/sdk" CACHE PATH "HIKvision SDK root")

# ============================================================================
# Include Directories
# ============================================================================
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()
include_directories(${CMAKE_SOURCE_DIR})

# ============================================================================
# Core Utility Libraries (No external dependencies)
# ============================================================================

# Logger - Professional logging system
add_library(imu_logger src/utils/Logger.cpp)
target_link_libraries(imu_logger PRIVATE Threads::Threads)

# ThreadSafeQueue - Header-only, no compilation needed
# Just include it in targets that need it

# ============================================================================
# Hardware Abstraction Layer
# ============================================================================

# Mock IMU Reader (for testing without hardware)
add_library(mock_imu_reader test/MockIMUReader.cpp)
target_link_libraries(mock_imu_reader PUBLIC imu_logger Threads::Threads)
target_include_directories(mock_imu_reader PUBLIC ${CMAKE_SOURCE_DIR})

# Mock Camera Control (for testing without hardware)
add_library(mock_camera_control test/MockCameraControl.cpp)
target_link_libraries(mock_camera_control PUBLIC imu_logger)
target_include_directories(mock_camera_control PUBLIC ${CMAKE_SOURCE_DIR})

# Real IMU Reader (when hardware available)
if(NOT BUILD_WITH_MOCK_HARDWARE)
    add_library(real_imu_reader src/hardware/IMUReader.cpp)
    target_link_libraries(real_imu_reader PUBLIC imu_logger Threads::Threads)
    target_include_directories(real_imu_reader PUBLIC ${CMAKE_SOURCE_DIR})
    
    # Real Camera Control (when hardware available)
    add_library(real_camera_control src/hardware/CameraControl.cpp)
    target_link_libraries(real_camera_control PUBLIC imu_logger)
    target_include_directories(real_camera_control PUBLIC ${CMAKE_SOURCE_DIR})
endif()

# ============================================================================
# Configuration Management
# ============================================================================

add_library(config_manager src/config/ConfigManager.cpp)
target_link_libraries(config_manager PUBLIC imu_logger)
target_include_directories(config_manager PUBLIC ${CMAKE_SOURCE_DIR})

# ============================================================================
# Application Layer
# ============================================================================

add_library(imu_camera_app src/app/Application.cpp)

if(BUILD_WITH_MOCK_HARDWARE)
    target_link_libraries(imu_camera_app PUBLIC 
        imu_logger
    )
    if(OpenCV_FOUND)
        target_link_libraries(imu_camera_app PUBLIC ${OpenCV_LIBS})
    endif()
else()
    target_link_libraries(imu_camera_app PUBLIC 
        imu_logger
        config_manager
        real_imu_reader
        real_camera_control
        Threads::Threads
    )
    if(OpenCV_FOUND)
        target_link_libraries(imu_camera_app PUBLIC ${OpenCV_LIBS})
    endif(    real_imu_reader
        real_camera_control
        Threads::Threads
        ${OpenCV_LIBS}
    )
endif()

target_include_directories(imu_camera_app PUBLIC ${CMAKE_SOURCE_DIR})

# ============================================================================
# Production Executable (Optional)
# ============================================================================

if(BUILD_PRODUCTION_APP)
    add_executable(imu_camera_new src/app/main_new.cpp)

    if(BUILD_WITH_MOCK_HARDWARE)
        target_link_libraries(imu_camera_new PRIVATE 
            imu_camera_app
            imu_logger
            config_manager
            mock_imu_reader
            mock_camera_control
            Threads::Threads
        )
    else()
        target_link_libraries(imu_camera_new PRIVATE 
            imu_camera_app
            imu_logger
            config_manager
            real_imu_reader
            real_camera_control
            Threads::Threads
        )
    endif()

    target_include_directories(imu_camera_new PRIVATE ${CMAKE_SOURCE_DIR})
endif()

# ============================================================================
# Test Executables (Only if BUILD_TESTS is ON)
# ============================================================================

if(BUILD_TESTS)
    # Hardware Mock Test Suite
    add_executable(test_hardware_mock test/test_hardware_main.cpp)
    target_link_libraries(test_hardware_mock PRIVATE
        imu_logger
        mock_imu_reader
        mock_camera_control
        Threads::Threads
    )
    target_include_directories(test_hardware_mock PRIVATE ${CMAKE_SOURCE_DIR})
    
    # Mark test executable
    set_target_properties(test_hardware_mock PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
    )
    
    # Enable CTest
    enable_testing()
    add_test(NAME HardwareMockTest COMMAND test_hardware_mock)
endif()

# ============================================================================
# Build Summary
# ============================================================================

message(STATUS "")
message(STATUS "╔════════════════════════════════════════════════════════════╗")
message(STATUS "║         ImuCamera Build Configuration Summary              ║")
message(STATUS "╚════════════════════════════════════════════════════════════╝")
message(STATUS "")
message(STATUS "Build Type:")
message(STATUS "  - Mock Hardware:    ${BUILD_WITH_MOCK_HARDWARE}")
message(STATUS "  - Build Tests:      ${BUILD_TESTS}")
if(OpenCV_FOUND)
    message(STATUS "  - OpenCV:           Found (${OpenCV_VERSION})")
else()
    message(STATUS "  - OpenCV:           NOT FOUND (optional, needed for vision processing)")
endif()
if(BUILD_PRODUCTION_APP)
    message(STATUS "  - imu_camera_new:       Main application (PRODUCTION)")
else()
    message(STATUS "  - imu_camera_new:       (disabled, use -DBUILD_PRODUCTION_APP=ON)")
endif()
message(STATUS "Targets:")
message(STATUS "  - imu_camera_new:       Main application")
if(BUILD_TESTS)
    message(STATUS "  - test_hardware_mock:   Hardware test suite (no physical hardware needed)")
endif()
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  - OpenCV:           ${OpenCV_FOUND}")
message(STATUS "  - Threads:          ${CMAKE_THREAD_LIBS_INIT}")
message(STATUS "")
message(STATUS "To build:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake ..")
message(STATUS "  make")
message(STATUS "")
if(BUILD_TESTS)
    message(STATUS "To run tests:")
    message(STATUS "  ./test/test_hardware_mock")
    message(STATUS "  OR: ctest")
    message(STATUS "")
endif()
message(STATUS "═══════════════════════════════════════════════════════════════")