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

    void(*waitSimulatedTime)(int DURATION_MS);

protected:
    // override because we have different time systems depending on hardware, sim, etc
    bool _sleep(int DURATION_MS) override;

};
#endif
