/*********************************
 * rem.h
 *
 * C++ implementation of REgression-based network Monitoring
 * Formerly known as NetworkMonitor.h***
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

#include "network_monitor.h"
#include "rem_model.h"
#include "../aodv/RoutingProtocol.h"

#include <vector>

class REM : public NetworkMonitor
{
public:
    //  static const int INIT_COUNT = 3; // DEPRECATED. REAL INIT_COUNT IN MODEL
    static const int HOP_COUNT = 2;

    REM() : NetworkMonitor() { simStartTime = getCurrentTimeMS(); }
    REM(IP_ADDR nodeIp) : NetworkMonitor(nodeIp) { simStartTime = getCurrentTimeMS(); }

    // NS3-TODO:  void initialize(Ptr<Node> parent, Ptr<EnergySource> battery, Ptr<Socket> socket); // initialize node 'parent' with network monitoring service
    // temp:
    void initialize(IP_ADDR parentIp);
    void initializeBatteryModel();
    void initializeRssModel(IP_ADDR pairIp);

    // get monitoring information 
    double getBatteryLevel(IP_ADDR ownerIp);
    double getRSSBetweenNodes(IP_ADDR pairIp, IP_ADDR ownerIp);

    // update local models with new data points 
    void updateLocalBatteryModel(double batteryLevel);
    void updateLocalRSSModel(IP_ADDR pairIp, double rss);

    // updating local models might result in drastic changes that need to be broadcasted...
    void sendUpdatedModel(PredictionModel* model, IP_ADDR dest);

    // update network models
    void updateNetworkBatteryModel(IP_ADDR ownerIp, BatteryModel model);
    void updateNetworkRSSModel(IP_ADDR ownerIp, IP_ADDR pairIp, RssModel model);

    // abstract function for getting current battery level
    virtual double getCurrentBatteryLevel() = 0;

    RoutingProtocol* routing;

// NS3-TODO: abstractize?    Ptr<Socket> socket;
protected:
    // get the current time in milliseconds. abstract function for sim and hardware implementations
    virtual uint32_t getCurrentTimeMS() = 0;
    uint32_t simStartTime;

    // local models: only one battery model but multiple RSS models
    BatteryModel localBatteryModel;
    // the owner is always this node, the IP_ADDR is the ip address of the paired node 
    map<IP_ADDR,RssModel> localRssModels; 

    // network models: models created by other nodes 
    map<IP_ADDR,BatteryModel> netBatteryModels;
    // int key is the ownerId to a vector of pairIds with associated RssModels
    map<IP_ADDR,map<IP_ADDR,RssModel>> netRssModels;

/* NS3-TODO:  
    Ptr<Node> parentNode; // parent node
    Ptr<EnergySource> battery;
*/
};


