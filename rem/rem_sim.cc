#include "rem_sim.h"

double REMSim::getCurrentBatteryLevel()
{
    return getSimulatedBatteryLevel(this->m_parentIp);
} 

uint32_t REMSim::getCurrentTimeMS()
{
    return getSimulatedTime();
}
