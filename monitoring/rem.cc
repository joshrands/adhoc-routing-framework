#include "rem.h"
#include "assert.h"
#include <chrono>
#include <ctime>
#include <string.h>

void REM::initialize(int parentId)
{
    this->parentId = parentId;

    if (REM_DEBUG)
        cout << "Initializing REM monitoring service for node " << this->parentId << endl;

    localBatteryModel.HOP_COUNT = HOP_COUNT;
    localBatteryModel.modelParameters.UPDATE_FREQUENCY = localBatteryModel.UPDATE_FREQUENCY;

    // initialize this node's battery model
    initializeBatteryModel();
}

void REM::initializeBatteryModel()
{
    if (localBatteryModel.getDataCount() < localBatteryModel.INIT_COUNT)
    {
        localBatteryModel.setState(ModelState::INIT);
        if (BATTERY_DEBUG)
            cout << "Insufficient data to build battery model" << endl;

        // this model is for parent of this network monitoring object 
        localBatteryModel.ownerId = getParentId();
        localBatteryModel.initialize();
    }
    else
    {
        if (BATTERY_DEBUG)
            cout << "This model should not be being initialized" << endl;
    }
}

void REM::initializeRssModel(int pairId)
{
    // create a new model
    RssModel model;

    model.ownerId = parentId; 
    model.pairId = pairId;
    model.dataCount = 0;
// NS3-TODO        model.parentNode = receiveNode;
    model.HOP_COUNT = this->HOP_COUNT;

    model.initialize();
    if (REM_DEBUG)
        cout << "[DEBUG] Local RSS model initialized." << endl;

    localRssModels[pairId] = model;
}

double REM::getBatteryLevel(int ownerId = -1)
{
    if (ownerId == -1)
    {
        return localBatteryModel.getDataPoint(getCurrentTimeMS());
    }
    else 
    {
        return netBatteryModels[ownerId].getDataPoint(getCurrentTimeMS());
    }
}

double REM::getRSSBetweenNodes(int pairId, int ownerId = -1)
{
    if (ownerId == -1)
    {
        // this is a local model
        return localRssModels[pairId].getDataPoint(getCurrentTimeMS());
    }
    else 
    {
        netRssModels[ownerId][pairId].getDataPoint(getCurrentTimeMS());        
    }
}

void REM::updateLocalBatteryModel(double batteryLevel)
{
    // might call broadcast model...
    localBatteryModel.addDataPoint(batteryLevel, getCurrentTimeMS());
}

// TODO: IP_ADDRRRRRR
void REM::updateLocalRSSModel(int pairId, double rss)
{
    // check if node already has this model
    if (localRssModels.count(pairId) <= 0)
    {
        initializeRssModel(pairId);
    }

    localRssModels[pairId].addDataPoint(rss, getCurrentTimeMS());
}

void REM::updateNetworkBatteryModel(int ownerId, BatteryModel model)
{
    netBatteryModels[ownerId] = model;
}

void REM::updateNetworkRSSModel(int ownerId, int pairId, RssModel model)
{
    netRssModels[ownerId][pairId] = model;
}

void REM::sendUpdatedModel(PredictionModel* model, IP_ADDR dest)
{
    /********8********16********24********32
     * NodeID|   Time to live   |Model Type
     *             Mu (float)
     *            Beta (float)
     *           Sigma (float)
    ***************************************/

    // allocate 16 bytes for a model packet
    char* buffer = (char*)(malloc(16));

    memcpy(buffer, &(model->ownerId), 1);
    buffer++;
    memcpy(buffer, &(model->DEFAULT_TTL), 2);
    buffer+=2;
    memcpy(buffer, &(model->MODEL_TYPE), 1);
    buffer++;
    memcpy(buffer, &(model->modelParameters.mu), 4);
    buffer+=4;
    memcpy(buffer, &(model->modelParameters.beta), 4);
    buffer+=4;
    memcpy(buffer, &(model->modelParameters.sigma), 4);
    buffer-=12;

    routing->socketSendPacket(buffer, 16, getIpFromString(BROADCAST), RoutingProtocol::DATA_PORT);

    delete buffer;
}
