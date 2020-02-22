/**
 * @file hardware_hello.h
 * @author Zachary Smeton (zsmeton@yahoo.com)
 * @brief Implementation of hello messages for hardware
 * @version 0.1
 * @date 2020-02-21
 */

#ifndef HARDWARE_HELLO_H
#define HARDWARE_HELLO_H

#include <chrono>
#include <thread>
#include "hello_monitor.h"

class HardwareHelloMonitor : public HelloMonitor{
public: 
    HardwareHelloMonitor(int portId, RoutingProtocol* routing) : HelloMonitor(portId, routing) { }

protected:
    bool _sleep(uint64_t duration_ms) override;
    uint64_t _getCurrentTimeMS() override;
};


class HHMTest : public HardwareHelloMonitor{
public:
    HHMTest(int portId, RoutingProtocol* routing) : HardwareHelloMonitor(portId, routing) { }
    bool sleep(uint64_t duration_ms);
    uint64_t getCurrentTimeMS();
};


#endif