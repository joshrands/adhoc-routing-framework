#include "rem_sim.h"

double REMSim::getCurrentBatteryLevel()
{
    return getSimulatedBatteryLevel(this->m_parentIp);
} 

uint64_t REMSim::_getCurrentTimeMS()
{
    return getSimulatedTime();
}
