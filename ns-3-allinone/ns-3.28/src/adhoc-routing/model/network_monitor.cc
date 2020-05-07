#include "network_monitor.h"

#include <iostream>

using namespace std;

NetworkMonitor::NetworkMonitor()
{
    if (MONITOR_DEBUG)
        cout << "[ERROR]: New network monitoring service for unassigned node" << endl;

    m_parentIp = 0;
}

NetworkMonitor::NetworkMonitor(IP_ADDR nodeIp)
{
    m_parentIp = nodeIp;

    if (MONITOR_DEBUG)
        cout << "[MONITOR]:[DEBUG]: New monitoring service for Node " << m_parentIp << endl;

    // start thread for local data monitoring 
    // start this thread for hardware implementations... 
//    localUpdateThread = thread(runLocalModelUpdateThread, this, &(this->localUpdateMutex));
//    localUpdateThread.detach();
}

NetworkMonitor::~NetworkMonitor()
{
}

bool NetworkMonitor::localDataExistsForNode(IP_ADDR nodeIp)
{
    if (localMonitoringData.count(nodeIp) > 0)
        return true;
    else
        return false;
}

bool NetworkMonitor::pairDataExistsForNode(IP_ADDR nodeIp)
{
    if (pairMonitoringData.count(nodeIp) > 0)
        return true;
    else 
        return false;
}

bool NetworkMonitor::isNodeOneHopNeighbor(IP_ADDR nodeIp)
{
    // if this node has pair monitoring data paired with me 
    vector<pair_data> localPairData = pairMonitoringData[m_parentIp];

    for (pair_data data : localPairData)
    {
        if (MONITOR_DEBUG)
            cout << "[MONITOR]:[DEBUG]: Node " << data.pairIp << " is a neighbor" << endl;

        if (data.pairIp == nodeIp)
            return true;
    }

    return false;
}

local_data NetworkMonitor::getLocalDataForNode(IP_ADDR nodeIp)
{
    return localMonitoringData[nodeIp];
}

pair_data NetworkMonitor::getPairDataBetweenNodes(IP_ADDR nodeIp, IP_ADDR ownerIp = -1)
{
    // the default is with this node 
    if (signed(ownerIp) == -1)
        ownerIp = m_parentIp;

    // we are looking at models from the ownderNodeId
    vector<pair_data> pairData = pairMonitoringData[ownerIp];
    for (pair_data data : pairData)
    {
        if (data.pairIp == nodeIp)
            return data;
    }

    pair_data empty;
    return empty;
}

void NetworkMonitor::runClock(int duration)
{
    m_clock_MS += duration;
}

void runLocalModelUpdateThread(NetworkMonitor* monitor, mutex* mux)
{
    // use mutex to protect memory
    mux->lock();

    if (monitor == NULL)
    {
        mux->unlock();
        terminate();
    }

    monitor->updateLocalModels();
    this_thread::sleep_for(chrono::milliseconds(LOCAL_DATA_UPDATE_RATE_MS));
    monitor->runClock(LOCAL_DATA_UPDATE_RATE_MS);

    mux->unlock();

    runLocalModelUpdateThread(monitor, mux);
}
