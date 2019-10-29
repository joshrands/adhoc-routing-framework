/*********************************
 * rem.h
 *
 * C++ implementation of REgression-based network Monitoring
 * Formerly known as NetworkMonitor.h***
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

#include "rem_model.h"
#include "rem_packet.h"

#include <vector>

class NetworkMonitor
{
public:
    //  static const int INIT_COUNT = 3; // DEPRECATED. REAL INIT_COUNT IN MODEL
    static const int HOP_COUNT = 2;

    NetworkMonitor(); // default constructor
    NetworkMonitor(int nodeId); // constructor with node id

    // NS3-TODO:  void initialize(Ptr<Node> parent, Ptr<EnergySource> battery, Ptr<Socket> socket); // initialize node 'parent' with network monitoring service
    // temp:
    void initialize(int parentId);
    void initializeBatteryModel();
    void initializeRssModel();

    // get state of different models
    ModelState getBatteryModelState();
    ModelState getRssModelState();

    int getParentId() { return this->parentId; }

    // local models: only one battery model but multiple RSS models
    BatteryModel batteryModel;
    vector<RssModel*> localRssModels; // different models for every node within comm range

    //vector<PredictionModel> models;
    vector<BatteryModel> batteryModels;
    vector<RssModel*> netRssModels;

// NS3-TODO: abstractize?    Ptr<Socket> socket;

private:
    int parentId; // id of parent node

/* NS3-TODO:  
    Ptr<Node> parentNode; // parent node
    Ptr<EnergySource> battery;
*/

    // create new models, return true if success false if need more data
    bool createNewBatteryModel();
    bool createNewRssModel();

    char* createBatteryModelPacket();
    char* createRssModelPacket();
};

