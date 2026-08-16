#pragma once

#include <zmq.hpp>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstring>

#include "../include/interface/MiddlewareInterface.h"
#include "../include/ImuData.h"
#include "../include/PTZCommand.h"
#include "../src/utils/Logger.h"

class ZmqMiddleware : public IMiddleware {
public:
    ZmqMiddleware()
        : context_Imu(1),
          context_PTZ(1),
          publisher_Imu(context_Imu, zmq::socket_type::pub),
          pull_socket_Imu(context_Imu, zmq::socket_type::pull),
          publisher_PTZ(context_PTZ, zmq::socket_type::pub),
          pull_socket_PTZ(context_PTZ, zmq::socket_type::pull) {
        publisher_Imu.bind("tcp://*:5555");
        pull_socket_Imu.bind("tcp://*:5556");
        publisher_PTZ.bind("tcp://*:5557");
        pull_socket_PTZ.bind("tcp://*:5558");
        // Give subscribers/pushers time to connect before the first send,
        // otherwise PUB drops messages sent before a SUB has connected
        // (the "slow joiner" problem) - same fix as sensor_node/app_node.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    ~ZmqMiddleware() override {
        publisher_Imu.close();
        pull_socket_Imu.close();
        publisher_PTZ.close();
        pull_socket_PTZ.close();
        context_Imu.close();
        context_PTZ.close();
    }

    void publishImuData(const ImuData& data) override {
        if(data.yaw < -180.0f || data.yaw > 180.0f ||
           data.pitch < -90.0f || data.pitch > 90.0f ||
           data.roll < -180.0f || data.roll > 180.0f) {
            utils::Logger::getInstance().error("Error: ImuData values out of range.");
            return;
        }
        zmq::message_t msg(&data, sizeof(ImuData));
        publisher_Imu.send(msg, zmq::send_flags::none);
        utils::Logger::getInstance().debug("Published ImuData: yaw=" + std::to_string(data.yaw) +
                  ", pitch=" + std::to_string(data.pitch) +
                  ", roll=" + std::to_string(data.roll));
    }

    void receiveImuData(ImuData& data) override {
        zmq::message_t msg;
        pull_socket_Imu.recv(msg, zmq::recv_flags::none);
        if (msg.size() != sizeof(ImuData)) {
            utils::Logger::getInstance().error("Received message size does not match ImuData size.");
            return;
        }
        std::memcpy(&data, msg.data(), sizeof(ImuData));
        if(data.yaw < -180.0f || data.yaw > 180.0f ||
           data.pitch < -90.0f || data.pitch > 90.0f ||
           data.roll < -180.0f || data.roll > 180.0f) {
            utils::Logger::getInstance().error("Error: received ImuData values out of range.");
            return;
        }
        utils::Logger::getInstance().debug("Received ImuData: yaw=" + std::to_string(data.yaw) +
                  ", pitch=" + std::to_string(data.pitch) +
                  ", roll=" + std::to_string(data.roll));
    }
    
    void sendPTZCommand(const PTZCommand& command) override {
        if(command.pan < -180.0f || command.pan > 180.0f ||
           command.tilt < -90.0f || command.tilt > 90.0f ||
           command.zoom < 1.0f || command.zoom > 10.0f) {
            utils::Logger::getInstance().error("Error: PTZCommand values out of range.");
            return;
        }
        zmq::message_t msg(&command, sizeof(PTZCommand));
        publisher_PTZ.send(msg, zmq::send_flags::none);
        utils::Logger::getInstance().debug("Sent PTZCommand: pan=" + std::to_string(command.pan) +
                  ", tilt=" + std::to_string(command.tilt) +
                  ", zoom=" + std::to_string(command.zoom));
    }

    void receivePTZCommand(PTZCommand& command) override {
        zmq::message_t msg;
        pull_socket_PTZ.recv(msg, zmq::recv_flags::none);
        if (msg.size() != sizeof(PTZCommand)) {
            utils::Logger::getInstance().error("Received message size does not match PTZCommand size.");
            return;
        }
        std::memcpy(&command, msg.data(), sizeof(PTZCommand));
        if(command.pan < -180.0f || command.pan > 180.0f ||
           command.tilt < -90.0f || command.tilt > 90.0f ||
           command.zoom < 1.0f || command.zoom > 10.0f) {
            utils::Logger::getInstance().error("Error: received PTZCommand values out of range.");
            return;
        }
        utils::Logger::getInstance().debug("Received PTZCommand: pan=" + std::to_string(command.pan) +
                  ", tilt=" + std::to_string(command.tilt) +
                  ", zoom=" + std::to_string(command.zoom));
    }

private:
    zmq::context_t context_Imu;
    zmq::context_t context_PTZ;
    zmq::socket_t publisher_Imu;
    zmq::socket_t pull_socket_Imu;
    zmq::socket_t publisher_PTZ;
    zmq::socket_t pull_socket_PTZ;
};
