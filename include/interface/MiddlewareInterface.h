#pragma once

#include "../ImuData.h"
#include "../PTZCommand.h"

class IMiddleware{
public:
    virtual void publishImuData(const ImuData& data) = 0;
    virtual void sendPTZCommand(const PTZCommand& command) = 0;
    virtual void receiveImuData(ImuData& data) = 0;
    virtual void receivePTZCommand(PTZCommand& command) = 0;

    virtual ~IMiddleware() = default;

};