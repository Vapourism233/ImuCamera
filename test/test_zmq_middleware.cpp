// Mock integration test for ZmqMiddleware.
//
// ZmqMiddleware binds 4 sockets (PUB+PULL for IMU, PUB+PULL for PTZ).
// This test spins up a "peer" thread using raw zmq sockets that connects
// to those 4 ports, mimicking what a real sensor/app node would do, and
// verifies data survives a round trip in both directions:
//
//   main thread (ZmqMiddleware)          peer thread (raw zmq)
//   ----------------------------          ---------------------
//   publishImuData(imuOut)      -- PUB -->  SUB  -> recv, compare
//   sendPTZCommand(ptzOut)      -- PUB -->  SUB  -> recv, compare
//   receiveImuData(imuIn)       <-- PUSH --  PUSH <- send
//   receivePTZCommand(ptzIn)    <-- PUSH --  PUSH <- send

#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "../middleware/ZmqMiddleware.h"

namespace {

bool imuEqual(const ImuData& a, const ImuData& b) {
    return a.yaw == b.yaw && a.pitch == b.pitch && a.roll == b.roll;
}

bool ptzEqual(const PTZCommand& a, const PTZCommand& b) {
    return a.pan == b.pan && a.tilt == b.tilt && a.zoom == b.zoom;
}

} // namespace

int main() {
    bool ok = true;

    ImuData imuOut{12.5f, -3.0f, 45.0f};
    PTZCommand ptzOut{10.0f, -5.0f, 2.0f};
    ImuData imuIn{};
    PTZCommand ptzIn{};

    ImuData imuReceivedByPeer{};
    PTZCommand ptzReceivedByPeer{};

    // Peer thread: plays the role of a remote node talking to ZmqMiddleware.
    std::thread peer([&]() {
        zmq::context_t peerCtx(1);
        zmq::socket_t subImu(peerCtx, zmq::socket_type::sub);
        zmq::socket_t subPtz(peerCtx, zmq::socket_type::sub);
        zmq::socket_t pushImu(peerCtx, zmq::socket_type::push);
        zmq::socket_t pushPtz(peerCtx, zmq::socket_type::push);

        subImu.connect("tcp://127.0.0.1:5555");
        subPtz.connect("tcp://127.0.0.1:5557");
        pushImu.connect("tcp://127.0.0.1:5556");
        pushPtz.connect("tcp://127.0.0.1:5558");
        subImu.set(zmq::sockopt::subscribe, "");
        subPtz.set(zmq::sockopt::subscribe, "");

        // Let the SUB sockets finish connecting/subscribing before the
        // main thread publishes - otherwise messages sent before the
        // subscription is established get silently dropped ("slow joiner").
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        zmq::message_t imuMsg;
        subImu.recv(imuMsg, zmq::recv_flags::none);
        std::memcpy(&imuReceivedByPeer, imuMsg.data(), sizeof(ImuData));

        zmq::message_t ptzMsg;
        subPtz.recv(ptzMsg, zmq::recv_flags::none);
        std::memcpy(&ptzReceivedByPeer, ptzMsg.data(), sizeof(PTZCommand));

        ImuData imuToSend{-20.0f, 15.0f, -60.0f};
        pushImu.send(zmq::message_t(&imuToSend, sizeof(ImuData)), zmq::send_flags::none);

        PTZCommand ptzToSend{-30.0f, 20.0f, 5.0f};
        pushPtz.send(zmq::message_t(&ptzToSend, sizeof(PTZCommand)), zmq::send_flags::none);
    });

    ZmqMiddleware mw; // binds all 4 ports, sleeps 200ms for slow-joiner

    // Give the peer's SUB sockets time to connect + subscribe before we publish.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    mw.publishImuData(imuOut);
    mw.sendPTZCommand(ptzOut);

    mw.receiveImuData(imuIn);
    mw.receivePTZCommand(ptzIn);

    peer.join();

    std::cout << "--- ZmqMiddleware mock test ---" << std::endl;

    if (imuEqual(imuReceivedByPeer, imuOut)) {
        std::cout << "[PASS] publishImuData -> peer SUB received matching data" << std::endl;
    } else {
        std::cout << "[FAIL] publishImuData mismatch" << std::endl;
        ok = false;
    }

    if (ptzEqual(ptzReceivedByPeer, ptzOut)) {
        std::cout << "[PASS] sendPTZCommand -> peer SUB received matching data" << std::endl;
    } else {
        std::cout << "[FAIL] sendPTZCommand mismatch" << std::endl;
        ok = false;
    }

    ImuData expectedImuIn{-20.0f, 15.0f, -60.0f};
    if (imuEqual(imuIn, expectedImuIn)) {
        std::cout << "[PASS] receiveImuData got data pushed by peer" << std::endl;
    } else {
        std::cout << "[FAIL] receiveImuData mismatch" << std::endl;
        ok = false;
    }

    PTZCommand expectedPtzIn{-30.0f, 20.0f, 5.0f};
    if (ptzEqual(ptzIn, expectedPtzIn)) {
        std::cout << "[PASS] receivePTZCommand got data pushed by peer" << std::endl;
    } else {
        std::cout << "[FAIL] receivePTZCommand mismatch" << std::endl;
        ok = false;
    }

    return ok ? 0 : 1;
}
