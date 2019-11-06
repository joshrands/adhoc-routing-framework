/*********************************
 * network_monitor.h
 *
 * Abstract implementation of network monitoring 
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

#include "monitor_info.h"
#include <vector>
#include <map>

using namespace std;
class NetworkMonitor
{
public:
    NetworkMonitor();
    NetworkMonitor(int nodeId);

    // various initialize functions for creating new models 
    virtual void initialize(int parentId) = 0;
    // abstract function for classes to update network monitoring data
    // can be done by asking for updates, using models, etc 
    virtual void updateNetworkMonitoringData() = 0;

    bool localDataExistsForNode(int nodeId);
    bool pairDataExistsForNode(int nodeId);

    // returns true if the nodeId is a one hop neighbor to this node 
    bool isNodeOneHopNeighbor(int nodeId);

    // get a local data prediction for a node 
    local_data getLocalDataForNode(int nodeId);
    // the default otherNodeId is this node
    pair_data getPairDataBetweenNodes(int nodeId, int ownerNodeId);

    int getParentId() { return this->parentId; }

protected:
    // node being monitored 
    int parentId;

    // list of nodes that this node has monitoring data for 
    map<int,local_data> localMonitoringData;
    map<int,vector<pair_data>> pairMonitoringData;
};
