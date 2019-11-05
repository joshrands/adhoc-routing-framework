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
#include "rem_packet.h"
#include "../aodv/RoutingProtocol.h"

#include <vector>

class REM : public NetworkMonitor
{
public:
    //  static const int INIT_COUNT = 3; // DEPRECATED. REAL INIT_COUNT IN MODEL
    static const int HOP_COUNT = 2;

    REM() : NetworkMonitor() { simStartTime = getCurrentTimeMS(); }
    REM(int nodeId) : NetworkMonitor(nodeId) { simStartTime = getCurrentTimeMS(); }

    // NS3-TODO:  void initialize(Ptr<Node> parent, Ptr<EnergySource> battery, Ptr<Socket> socket); // initialize node 'parent' with network monitoring service
    // temp:
    void initialize(int parentId);
    void initializeBatteryModel();
    void initializeRssModel(int pairId);

    // get monitoring information 
    double getBatteryLevel(int ownerId);
    double getRSSBetweenNodes(int pairId, int ownerId);

    // update local models with new data points 
    void updateLocalBatteryModel(double batteryLevel);
    void updateLocalRSSModel(int pairId, double rss);

    // updating local models might result in drastic changes that need to be broadcasted...
    void sendUpdatedModel(PredictionModel* model, IP_ADDR dest);

    // update network models
    void updateNetworkBatteryModel(int ownerId, BatteryModel model);
    void updateNetworkRSSModel(int ownerId, int pairId, RssModel model);

    // abstract function for getting current battery level
    virtual double getCurrentBatteryLevel() = 0;

    RoutingProtocol* routing;

// NS3-TODO: abstractize?    Ptr<Socket> socket;
protected:
    // get the current time in milliseconds. abstract function for sim and hardware implementations
    virtual uint32_t getCurrentTimeMS() = 0;
    uint32_t simStartTime;

    // local models: only one battery model but multiple RSS models
    BatteryModel batteryModel;
    // the owner is always this node, the int key is the pairId
    map<int,RssModel> localRssModels; 

    // network models: models created by other nodes 
    // int key is the ownerId
    map<int,BatteryModel> batteryModels;
    // int key is the ownerId to a vector of pairIds with associated RssModels
    map<int,vector<pair<int,RssModel>>> netRssModels;

/* NS3-TODO:  
    Ptr<Node> parentNode; // parent node
    Ptr<EnergySource> battery;
*/
};


