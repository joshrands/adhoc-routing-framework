#include "rem.h"
#include "assert.h"
#include <chrono>
#include <ctime>

void REM::initialize(int parentId)
{
    this->parentId = parentId;

    if (REM_DEBUG)
        cout << "Initializing REM monitoring service for node " << this->parentId << endl;

    batteryModel.HOP_COUNT = HOP_COUNT;
    batteryModel.modelParameters.UPDATE_FREQUENCY = batteryModel.UPDATE_FREQUENCY;

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
    // create an RSS model between this node and pairId
    // TODO: Look at battery initialize model
//    RssModel model;
//    localRssModels[pairId] = model;
}

double REM::getBatteryLevel(int ownerId = -1)
{

    return 0;
}

double REM::getRSSBetweenNodes(int pairId, int ownerId = -1)
{

    return 0;
}

void REM::updateLocalBatteryModel(double batteryLevel)
{
    // might call broadcast model...
}

void REM::updateLocalRSSModel(int pairId, double rss)
{
    // might call broadcast model...
}

void REM::sendUpdatedModel(PredictionModel* model, IP_ADDR dest)
{

}

void REM::updateNetworkBatteryModel(int ownerId, BatteryModel model)
{

}

void REM::updateNetworkRSSModel(int ownerId, int pairId, RssModel model)
{

}
