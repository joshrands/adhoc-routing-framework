#ifndef HELLOSIM_H
#define HELLOSIM_H

/*********************************
 * hello_sim.h
 *
 * C++ implementation of HelloMonitor for simulated environments
 * 
 * Author: Josh Rands
 * Date: 2/13/2019
 ********************************/

#include "hello_monitor.h"

class SimHelloMonitor : public HelloMonitor 
{
public:
    SimHelloMonitor(int portId, RoutingProtocol* routing) : HelloMonitor(portId, routing) { }

    void(*waitSimulatedTime)(int duration_ms);
    uint32_t(*getSimulatedTime)();

protected:
    // override because we have different time systems depending on hardware, sim, etc
    bool _sleep(int duration_ms) override;

    // override because we have different time systems depending on hardware, sim, etc
    uint32_t getCurrentTimeMS() override;

};
#endif
