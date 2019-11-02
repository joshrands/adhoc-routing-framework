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

using namespace std;
class NetworkMonitor
{
public:
    NetworkMonitor();
    NetworkMonitor(int nodeId);

    // various initialize functions for creating new models 
    virtual void initialize(int parentId) = 0;
    virtual void initializeBatteryModel() = 0;
    virtual void initializeRssModel(int pairId) = 0;

    virtual double getBatteryLevelOfThisNode() = 0;
    virtual double getRSSBetweenThisNodeAnd(int pairId) = 0;

    bool localDataExistsForNode(int nodeId);
    bool pairDataExistsForNode(int nodeId);

    // returns true if the nodeId is a one hop neighbor to this node 
    bool isNodeOneHopNeighbor(int nodeId);

    // get a local data prediction for a node 
    local_data getLocalDataForNode(int nodeId);
    // the default otherNodeId is this node
    pair_data getPairDataBetweenNodes(int nodeId, int otherNodeId);

    int getParentId() { return this->parentId; }

protected:
    // node being monitored 
    int parentId;

    // list of nodes that this node has monitoring data for 
    vector<int> localMonitoringData;
    vector<int> pairMonitoringData;
};

class REMNetworkMonitor : public NetworkMonitor
{
public:

protected:

};
