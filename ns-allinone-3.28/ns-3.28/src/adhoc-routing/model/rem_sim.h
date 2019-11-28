/*********************************
 * rem_sim.h
 *
 * C++ implementation of REM for simulated environments
 * Note: Current implementation only works for discrete event simulators
 * 
 * Author: Josh Rands
 * Date: 11/27/2019
 ********************************/

#include "rem.h"

class REMSim : public REM 
{
public:
    REMSim() : REM() { simStartTime = getCurrentTimeMS(); }
    REMSim(IP_ADDR parentIp) : REM(parentIp) { simStartTime = getCurrentTimeMS(); }

    double getCurrentBatteryLevel() override;

    // These functions much be paired with functions knowledgeable of global simulated data
    // Nodes will ONLY use the global knowledge to get THEIR OWN local data
    // NOTE: pair data is simulated in adhoc_routing. REMSim treats pair data the same as other environments
    function<double(IP_ADDR nodeIp)> getSimulatedBatteryLevel;
    function<uint32_t()> getSimulatedTime;

protected:
    // override because we have different time systems depending on hardware, sim, etc
    uint32_t getCurrentTimeMS() override;

};
