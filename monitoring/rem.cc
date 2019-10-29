#include "rem.h"
#include "assert.h"

void REM::initialize(int parentId)
{
    this->parentId = parentId;

    if (REM_DEBUG)
        cout << "Initializing REM monitoring service for node " << this->parentId << endl;

    batteryModel.HOP_COUNT = HOP_COUNT;
    batteryModel.model.UPDATE_FREQUENCY = batteryModel.UPDATE_FREQUENCY;

    // initialize this node's battery model
    initializeBatteryModel();
}

void REM::initializeBatteryModel()
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

void REM::initializeRssModel(int pairId)
{
    // TODO: Do this...
}

double REM::getBatteryLevel()
{
    return -1.0;
//    return batteryModel.getDataPoint()
}

double REM::getRSSBetweenNodes(int pairId)
{
    return -1.0;
}

ModelState REM::getBatteryModelState()
{
    return batteryModel.getState();
}
