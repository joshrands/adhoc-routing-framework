#include "rem.h"
#include "assert.h"

NetworkMonitor::NetworkMonitor()
{
    if (REM_DEBUG)
        cout << "New network monitoring service for unassigned node" << endl;

    parentId = 0;
}

NetworkMonitor::NetworkMonitor(int nodeId)
{
    this->parentId = nodeId;

    if (REM_DEBUG)
        cout << "New monitoring service for Node " << this->parentId << endl;
}

void NetworkMonitor::initialize(int parentId)
{
    this->parentId = parentId;

    if (REM_DEBUG)
        cout << "Initializing monitoring service for node " << this->parentId << endl;

    batteryModel.HOP_COUNT = HOP_COUNT;
    // NS3-TODO: Why is this commented out? 
    //  rssModel.HOP_COUNT = HOP_COUNT;

    batteryModel.model.UPDATE_FREQUENCY = batteryModel.UPDATE_FREQUENCY;
    //  rssModel.model.UPDATE_FREQUENCY = rssModel.UPDATE_FREQUENCY;

    initializeBatteryModel();
    initializeRssModel();
}

void NetworkMonitor::initializeBatteryModel()
{
    if (batteryModel.getDataCount() < batteryModel.INIT_COUNT)
    {
        batteryModel.setState(ModelState::INIT);
        if (BATTERY_DEBUG)
            cout << "Insufficient data to build battery model" << endl;

        /* NS3-TODO:
        batteryModel.setParentNode(this->parentNode);
        batteryModel.battery = this->battery;
        batteryModel.socket = this->socket;
        */
        batteryModel.initialize();
    }
    else
    {
        if (BATTERY_DEBUG)
            cout << "This model should not be being initialized" << endl;
    }
}

void NetworkMonitor::initializeRssModel()
{
    // this happens dynamically,when a node receives a packet from another node in the network create necessary data structures
    cout << "Why am I here?" << endl;
    assert(0 == -1);
}

ModelState NetworkMonitor::getBatteryModelState()
{
    return batteryModel.getState();
}
