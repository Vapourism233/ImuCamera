// Application layer - minimal client.
//
// Responsibility (for now, intentionally kept simple): read a PTZ command
// from the user on stdin, and pass it to the middle layer (ZmqMiddleware)
// by pushing it into the PTZ PULL socket that ZmqMiddleware binds.
//
// This process does NOT own/bind any sockets itself - it only connects to
// the middleware, which must already be running.

#include <zmq.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "../include/PTZCommand.h"

int main() {
    zmq::context_t context(1);
    zmq::socket_t push_socket(context, zmq::socket_type::push);
    push_socket.connect("tcp://127.0.0.1:5558");

    std::cout << "Application Layer: enter a PTZ command as '<pan> <tilt> <zoom>'"
              << " (or 'q' to quit)" << std::endl;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (line == "q" || line == "quit") {
            break;
        }

        std::istringstream iss(line);
        PTZCommand command{};
        if (!(iss >> command.pan >> command.tilt >> command.zoom)) {
            std::cout << "Invalid input. Expected: <pan> <tilt> <zoom>" << std::endl;
            continue;
        }

        zmq::message_t msg(&command, sizeof(PTZCommand));
        push_socket.send(msg, zmq::send_flags::none);
        std::cout << "Sent PTZCommand: pan=" << command.pan
                  << ", tilt=" << command.tilt
                  << ", zoom=" << command.zoom << std::endl;
    }

    return 0;
}
