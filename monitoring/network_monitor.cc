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


