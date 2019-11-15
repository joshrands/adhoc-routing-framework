#include "rem.h"
#include "assert.h"
#include <chrono>
#include <ctime>
#include <string.h>

void REM::initialize(IP_ADDR parentIp)
{
    m_parentIp = parentIp;

    if (REM_DEBUG)
        cout << "[DEBUG]: Initializing REM monitoring service for node " << getStringFromIp(m_parentIp) << endl;

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
            cout << "[WARNING]: This model should not be being initialized" << endl;
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
        cout << "[DEBUG]: Local RSS model initialized." << endl;

    model.setState(ModelState::STABLE);

    localRssModels[pairIp] = model;
}

void REM::updateLocalModels()
{
    localMonitoringData[m_parentIp].batteryLevel = getBatteryLevel();

    // clear pair monitoring data and update with new 
    pairMonitoringData.clear();

    auto it = localRssModels.begin();
    while (it != localRssModels.end())
    {
        if (it->second.getState() == ModelState::STABLE)
        {
            pair_data data;
            data.pairIp = it->first;
            data.rss = it->second.getDataPoint(getCurrentTimeMS());

            pairMonitoringData[m_parentIp].push_back(data);
        }

        it++;
    }
}

void REM::handleMonitoringPacketBuffer(char* packet, int length, IP_ADDR source, int port)
{
    // current packet structure (double check sendUpdatedModel) 

    /********8********16********24********32
     *          Node IP Address
     * Resrv |   Time to live   |Model Type
     *             Mu (float)
     *            Beta (float)
     *           Sigma (float)
     *          Pair IP (Optional)
    ***************************************/

    // decode the packet 
    ModelParameters params;

    IP_ADDR ownerIp;
    memcpy(&ownerIp, packet, 4);
    params.ownerId = ownerIp;

    memcpy(&(params.timeToLive), &(packet[5]), 2);

    uint8_t type;
    memcpy(&type, &(packet[7]), 1);

    params.type = (ModelType)(type);
    if (REM_DEBUG)
        cout << "[DEBUG]: Rem model type: " << params.type << endl;

    memcpy(&(params.mu), &(packet[8]), 4);
    memcpy(&(params.beta), &(packet[12]), 4);
    memcpy(&(params.sigma), &(packet[16]), 4);

    // Create model! 
    switch (params.type)
    {
        case ModelType::BATTERY:
            {
            if (REM_DEBUG)
                cout << "[DEBUG]: Adding network battery model for node " << getStringFromIp(ownerIp) << endl;

            BatteryModel batteryModel;
            batteryModel.modelParameters = params;
            batteryModel.ownerIp = ownerIp;
            batteryModel.setState(ModelState::STABLE);

            // add the model to the list of network models 
            netBatteryModels[ownerIp] = batteryModel;
            } 
            break;

        case ModelType::RSS:
            {
            // pair data! 
            IP_ADDR pairIp;
            memcpy(&(pairIp), &(packet[20]), 4);
            params.pairId = pairIp;

            if (REM_DEBUG) 
                cout << "[DEBUG]: Adding network rss model for node " << getStringFromIp(ownerIp) << " between " << getStringFromIp(pairIp) << endl;


            RssModel rssModel;
            rssModel.modelParameters = params;
            rssModel.ownerIp = ownerIp;
            rssModel.setState(ModelState::STABLE);

            netRssModels[ownerIp][pairIp] = rssModel;
            }

            break;

        default:
            cout << "[ERROR]: Unknown REM model type." << endl;
            break;
    }
}

double REM::getBatteryLevel(IP_ADDR ownerIp)
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
    // adding a new point might result in a new model...
    localBatteryModel.addDataPoint(batteryLevel, getCurrentTimeMS());

    // if the model needs to be broadcasted, do it! 
    if (localBatteryModel.needsToBeBroadcasted)
    {
        if (REM_DEBUG)
            cout << "[DEBUG]: Sending updated battery model from node " << getStringFromIp(m_parentIp) << endl;

        sendUpdatedModel(&localBatteryModel, getIpFromString(BROADCAST));
        // model has been broadcasted
        localBatteryModel.needsToBeBroadcasted = false;
    }
}

void REM::updateLocalRSSModel(IP_ADDR pairIp, double rss)
{
    // check if node already has this model
    if (localRssModels.count(pairIp) <= 0)
    {
        initializeRssModel(pairIp);
    }

    if (REM_DEBUG)
        cout << "[DEBUG]: Updating local RSS model between " << getStringFromIp(m_parentIp) << " and " << getStringFromIp(pairIp) << endl;

    // adding a data point might result in a new model...
    localRssModels[pairIp].addDataPoint(rss, getCurrentTimeMS());

    // if the model needs to be broadcasted, do it! 
    if (localRssModels[pairIp].needsToBeBroadcasted)
    {
        sendUpdatedModel(&(localRssModels[pairIp]), getIpFromString(BROADCAST));
        // model has been broadcasted
        localRssModels[pairIp].needsToBeBroadcasted = false;
    }
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
     *          Pair IP (Optional)
    ***************************************/
    
    REMModelPacket packet = model->createREMModelPacket();

    // allocate 20 bytes for a model packet
    int size = sizeof(packet);
    if (REM_DEBUG)
        cout << "[DEBUG]: Sending REM model packet. Size = " << size << endl;

    char* buffer = (char*)(malloc(size));
    memcpy(buffer, &packet, size);

    routing->socketSendPacket(buffer, size, getIpFromString(BROADCAST), MONITOR_PORT);

    delete buffer;
}

void REM::updatePairData(pair_data pairData)
{
    if (REM_DEBUG)
        cout << "[DEBUG]: Updating pair data for REM model" << endl;
        
    updateLocalRSSModel(pairData.pairIp, pairData.rss);
} 

double REMTest::getCurrentBatteryLevel()
{
    return m_battery;
} 

uint32_t REMTest::getCurrentTimeMS()
{
    return m_clock;
} 

void REMTest::runClock(int duration)
{
    m_clock -= duration;
}

void REMTest::drainBattery()
{
    m_battery -= 2;
}
