#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

/*********************************
 * network_monitor.h
 *
 * Abstract implementation of network monitoring 
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>

#include "monitor_info.h"
#include "adhoc_defines.h"

using namespace std;
class NetworkMonitor{
public:
    NetworkMonitor();
    NetworkMonitor(IP_ADDR nodeIp);
    ~NetworkMonitor();

    virtual void updatePairData(pair_data pairData) = 0;

    // various initialize functions for creating new models 
    virtual void initialize(IP_ADDR parentIp) = 0;
    // abstract function for classes to update network monitoring data
    // can be done by asking for updates, using models, etc 

    virtual void handleMonitoringPacketBuffer(char* packet, int length, IP_ADDR source, int port) = 0;
    virtual void updateLocalModels() = 0;

    // get monitoring information 
//    virtual double getBatteryLevel(IP_ADDR ownerIp) = 0;
//    virtual double getRSSBetweenNodes(IP_ADDR pairIp, IP_ADDR ownerIp) = 0;

    // update local models with new data points 
//    virtual void updateLocalBatteryModel(double batteryLevel) = 0;
//    virtual void updateLocalRSSModel(IP_ADDR pairIp, double rss) = 0;

    bool localDataExistsForNode(IP_ADDR nodeIp);
    bool pairDataExistsForNode(IP_ADDR nodeIp);

    // returns true if the nodeId is a one hop neighbor to this node 
    bool isNodeOneHopNeighbor(IP_ADDR nodeIp);

    // get a local data prediction for a node 
    local_data getLocalDataForNode(IP_ADDR nodeIp);
    // the default otherNodeId is this node
    pair_data getPairDataBetweenNodes(IP_ADDR nodeIp, IP_ADDR ownerIp);

    int getParentIp() { return this->m_parentIp; }

    void runClock(int duration = 1000);

protected:
    int m_clock_MS = 0;
    double m_battery = 100; 

    thread localUpdateThread;
    mutex localUpdateMutex;

    // node being monitored 
    IP_ADDR m_parentIp;

    // list of nodes that this node has monitoring data for 
    // these maps are updated from other data in the child classes for network monitoring 
    map<IP_ADDR,local_data> localMonitoringData;
    map<IP_ADDR,vector<pair_data>> pairMonitoringData;
};

void runLocalModelUpdateThread(NetworkMonitor* monitor, mutex* mux);

#endif