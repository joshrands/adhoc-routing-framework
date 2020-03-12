#include "hello_sim.h"

bool SimHelloMonitor::_sleep(uint64_t duration_ms)
{
    waitSimulatedTime(duration_ms);

    return true;
}

uint64_t SimHelloMonitor::_getCurrentTimeMS()
{
    return getSimulatedTime();
}
