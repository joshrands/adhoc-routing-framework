#include "network_monitor.h"
#include "defines.h"

#include <iostream>

using namespace std;

NetworkMonitor::NetworkMonitor()
{
    if (MONITOR_DEBUG)
        cout << "New network monitoring service for unassigned node" << endl;

    parentId = 0;
}

NetworkMonitor::NetworkMonitor(int nodeId)
{
    this->parentId = nodeId;

    if (MONITOR_DEBUG)
        cout << "New monitoring service for Node " << this->parentId << endl;
}

bool NetworkMonitor::localDataExistsForNode(int nodeId)
{
    if (localMonitoringData.count(nodeId) > 0)
        return true;
    else
        return false;
}

bool NetworkMonitor::pairDataExistsForNode(int nodeId)
{
    if (pairMonitoringData.count(nodeId) > 0)
        return true;
    else 
        return false;
}

bool NetworkMonitor::isNodeOneHopNeighbor(int nodeId)
{
    // if this node has pair monitoring data paired with me 
    vector<pair_data> localPairData = pairMonitoringData[parentId];

    for (pair_data data : localPairData)
    {
        if (data.pairId == nodeId)
            return true;
    }

    return false;
}

local_data NetworkMonitor::getLocalDataForNode(int nodeId)
{
    return localMonitoringData[nodeId];
}

pair_data NetworkMonitor::getPairDataBetweenNodes(int nodeId, int ownerNodeId = -1)
{
    // the default is with this node 
    if (ownerNodeId == -1)
        ownerNodeId = parentId;

    // we are looking at models from the ownderNodeId
    vector<pair_data> pairData = pairMonitoringData[ownerNodeId];
    for (pair_data data : pairData)
    {
        if (data.pairId == nodeId)
            return data;
    }
}


