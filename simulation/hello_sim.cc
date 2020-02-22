#include "hello_sim.h"

bool SimHelloMonitor::_sleep(int duration_ms)
{
    waitSimulatedTime(duration_ms);

    return true;
}

uint32_t SimHelloMonitor::getCurrentTimeMS()
{
    return getSimulatedTime();
}
