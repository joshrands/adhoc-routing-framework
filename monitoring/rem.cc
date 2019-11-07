#include "rem.h"
#include "assert.h"
#include <chrono>
#include <ctime>
#include <string.h>

void REM::initialize(IP_ADDR parentIp)
{
    m_parentIp = parentIp;

    if (REM_DEBUG)
        cout << "[DEBUG]: Initializing REM monitoring service for node " << m_parentIp << endl;

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
            cout << "[DEBUG]: Insufficient data to build battery model" << endl;

        // this model is for parent of this network monitoring object 
        localBatteryModel.ownerIp = getParentIp();
        localBatteryModel.initialize();
    }
    else
    {
        if (BATTERY_DEBUG)
            cout << "[DEBUG]: This model should not be being initialized" << endl;
    }
}

void REM::initializeRssModel(IP_ADDR pairIp)
{
    // create a new model
    RssModel model;

    model.ownerIp = m_parentIp; 
    model.pairIp = pairIp;
    model.dataCount = 0;
// NS3-TODO        model.parentNode = receiveNode;
    model.HOP_COUNT = this->HOP_COUNT;

    model.initialize();
    if (REM_DEBUG)
        cout << "[DEBUG]: [DEBUG] Local RSS model initialized." << endl;

    localRssModels[pairIp] = model;
}

double REM::getBatteryLevel(IP_ADDR ownerIp = -1)
{
    if (ownerIp == -1)
    {
        return localBatteryModel.getDataPoint(getCurrentTimeMS());
    }
    else 
    {
        return netBatteryModels[ownerIp].getDataPoint(getCurrentTimeMS());
    }
}

double REM::getRSSBetweenNodes(IP_ADDR pairIp, IP_ADDR ownerIp = -1)
{
    if (ownerIp == -1)
    {
        // this is a local model
        return localRssModels[pairIp].getDataPoint(getCurrentTimeMS());
    }
    else 
    {
        netRssModels[ownerIp][pairIp].getDataPoint(getCurrentTimeMS());        
    }
}

void REM::updateLocalBatteryModel(double batteryLevel)
{
    // might call broadcast model...
    localBatteryModel.addDataPoint(batteryLevel, getCurrentTimeMS());
}

void REM::updateLocalRSSModel(IP_ADDR pairIp, double rss)
{
    // check if node already has this model
    if (localRssModels.count(pairIp) <= 0)
    {
        initializeRssModel(pairIp);
    }

    localRssModels[pairIp].addDataPoint(rss, getCurrentTimeMS());
}

void REM::updateNetworkBatteryModel(IP_ADDR ownerIp, BatteryModel model)
{
    netBatteryModels[ownerIp] = model;
}

void REM::updateNetworkRSSModel(IP_ADDR ownerIp, IP_ADDR pairIp, RssModel model)
{
    netRssModels[ownerIp][pairIp] = model;
}

void REM::sendUpdatedModel(PredictionModel* model, IP_ADDR dest)
{
    /********8********16********24********32
     *          Node IP Address
     * Resrv |   Time to live   |Model Type
     *             Mu (float)
     *            Beta (float)
     *           Sigma (float)
    ***************************************/
    
    REMModelPacket packet = model->createREMModelPacket();

    // allocate 20 bytes for a model packet
    int size = sizeof(packet);

    char* buffer = (char*)(malloc(size));
    memcpy(buffer, &packet, size);

    routing->socketSendPacket(buffer, size, getIpFromString(BROADCAST), RoutingProtocol::DATA_PORT);

    delete buffer;
}
