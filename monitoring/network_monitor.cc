#include "network_monitor.h"
#include "monitor_defines.h"

#include <iostream>

using namespace std;

NetworkMonitor::NetworkMonitor()
{
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: New network monitoring service for unassigned node" << endl;

    m_parentIp = 0;
}

NetworkMonitor::NetworkMonitor(IP_ADDR nodeIp)
{
    m_parentIp = nodeIp;

    if (MONITOR_DEBUG)
        cout << "[DEBUG]: New monitoring service for Node " << m_parentIp << endl;
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
    if (ownerIp == -1)
        ownerIp = m_parentIp;

    // we are looking at models from the ownderNodeId
    vector<pair_data> pairData = pairMonitoringData[ownerIp];
    for (pair_data data : pairData)
    {
        if (data.pairIp == nodeIp)
            return data;
    }
}

