/*********************************
 * network_monitor.h
 *
 * Abstract implementation of network monitoring 
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

#include "monitor_info.h"
#include "../defines.h"
#include <vector>
#include <map>

using namespace std;
class NetworkMonitor
{
public:
    NetworkMonitor();
    NetworkMonitor(IP_ADDR nodeIp);

    // various initialize functions for creating new models 
    virtual void initialize(IP_ADDR parentIp) = 0;
    // abstract function for classes to update network monitoring data
    // can be done by asking for updates, using models, etc 
    virtual void updateNetworkMonitoringData() = 0;

    bool localDataExistsForNode(IP_ADDR nodeIp);
    bool pairDataExistsForNode(IP_ADDR nodeIp);

    // returns true if the nodeId is a one hop neighbor to this node 
    bool isNodeOneHopNeighbor(IP_ADDR nodeIp);

    // get a local data prediction for a node 
    local_data getLocalDataForNode(IP_ADDR nodeIp);
    // the default otherNodeId is this node
    pair_data getPairDataBetweenNodes(IP_ADDR nodeIp, IP_ADDR ownerIp);

    int getParentIp() { return this->m_parentIp; }

protected:
    // node being monitored 
    IP_ADDR m_parentIp;

    // list of nodes that this node has monitoring data for 
    map<IP_ADDR,local_data> localMonitoringData;
    map<IP_ADDR,vector<pair_data>> pairMonitoringData;
};
