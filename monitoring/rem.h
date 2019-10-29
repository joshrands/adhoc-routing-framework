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

#include <vector>

class REM : public NetworkMonitor
{
public:
    //  static const int INIT_COUNT = 3; // DEPRECATED. REAL INIT_COUNT IN MODEL
    static const int HOP_COUNT = 2;

    REM() : NetworkMonitor() { }
    REM(int nodeId) : NetworkMonitor(nodeId) { }

    // NS3-TODO:  void initialize(Ptr<Node> parent, Ptr<EnergySource> battery, Ptr<Socket> socket); // initialize node 'parent' with network monitoring service
    // temp:
    void initialize(int parentId);
    void initializeBatteryModel();
    void initializeRssModel(int pairId);

    // get monitoring information 
    double getBatteryLevel();
    double getRSSBetweenNodes(int pairId);

    // get state of different models
    ModelState getBatteryModelState();
    ModelState getRssModelState();
    // local models: only one battery model but multiple RSS models
    BatteryModel batteryModel;
    vector<RssModel*> localRssModels; // different models for every node within comm range
    // network models: models created by other nodes 
    vector<BatteryModel> batteryModels;
    vector<RssModel*> netRssModels;

// NS3-TODO: abstractize?    Ptr<Socket> socket;
protected:

/* NS3-TODO:  
    Ptr<Node> parentNode; // parent node
    Ptr<EnergySource> battery;
*/
};

