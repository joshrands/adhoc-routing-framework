#include "rem.h"
#include "assert.h"
#include <chrono>
#include <ctime>
#include <string.h>

void REM::initialize(IP_ADDR parentIp)
{
    m_parentIp = parentIp;
    simStartTime = this->getCurrentTimeMS();

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
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Updating local models" << endl;

    updateLocalBatteryModel(this->getCurrentBatteryLevel());

    // this function is being continously called by a thread and upon updating pair data 
    localMonitoringData[m_parentIp].batteryLevel = getBatteryLevel();

    // clear pair monitoring data and update with new 
    pairMonitoringData.clear();

    routing->resetLinks();

    auto it = localRssModels.begin();
    while (it != localRssModels.end())
    {
        if (it->second.getState() == ModelState::STABLE)
        {
            pair_data data;
            data.pairIp = it->first;
            data.rss = it->second.getDataPoint(getCurrentTimeMS());

            if (data.rss > RSS_OUT_OF_RANGE)
            {
                pairMonitoringData[m_parentIp].push_back(data);
                routing->addExistingLink(data.pairIp);
            }
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

    // buffer is in REMModelPacket format 
    REMModelPacket modelPacket;
    memcpy(&modelPacket, packet, sizeof(params));

    params.ownerId = modelPacket.parentIp;
    params.type = (ModelType)modelPacket.type;
    params.timeToLive = modelPacket.timeToLive;
    params.mu = modelPacket.mu;
    params.beta = modelPacket.beta;
    params.sigma = modelPacket.sigma;

    if (REM_DEBUG)
        cout << "[DEBUG]: REM model type: " << params.type << endl;

    // Create model! 
    switch (params.type)
    {
        case ModelType::BATTERY:
            {
            if (REM_DEBUG)
                cout << "[DEBUG]: Adding network battery model for node " << getStringFromIp(params.ownerId) << endl;

            BatteryModel batteryModel;
            batteryModel.modelParameters = params;
            batteryModel.ownerIp = params.ownerId;
            batteryModel.setState(ModelState::STABLE);

            // add the model to the list of network models 
            netBatteryModels[params.ownerId] = batteryModel;
            } 
            break;

        case ModelType::RSS:
            {
            // pair data! 
            params.pairId = modelPacket.pairIp;

            if (REM_DEBUG) 
                cout << "[DEBUG]: Adding network rss model for node " << getStringFromIp(params.ownerId) << " between " << getStringFromIp(params.pairId) << endl;

            RssModel rssModel;
            rssModel.modelParameters = params;
            rssModel.ownerIp = params.ownerId;
            rssModel.setState(ModelState::STABLE);

            netRssModels[params.ownerId][params.pairId] = rssModel;
            }

            break;

        default:
            cout << "[ERROR]: Unknown REM model type." << endl;
            break;
    }
}

double REM::getBatteryLevel(IP_ADDR ownerIp)
{
    if (signed(ownerIp) == -1)
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
    if (signed(ownerIp) == -1)
    {
        // this is a local model
        return localRssModels[pairIp].getDataPoint(getCurrentTimeMS());
    }
    else 
    {
        return netRssModels[ownerIp][pairIp].getDataPoint(getCurrentTimeMS());        
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

        sendUpdatedModel(&localBatteryModel, getIpFromString(BROADCAST_STR));
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
        sendUpdatedModel(&(localRssModels[pairIp]), getIpFromString(BROADCAST_STR));
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

    if (routing == nullptr)
        cout << "[ERROR]: NO ROUTING PROTOCOL" << endl;
    else 
    {
        char* buffer = (char*)(malloc(size));
        memcpy(buffer, &packet, size);

        routing->socketSendPacket(buffer, size, getIpFromString(BROADCAST_STR), MONITOR_PORT);

        delete buffer;
    }
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
    return m_clock_MS;
}

void REMTest::drainBattery()
{
    m_battery -= 2;
}
