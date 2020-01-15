#include "adhoc_routing.h"

void AdHocRouting::receivePacketWithPairData(char* packet, int length, IP_ADDR source, int port, pair_data pairData)
{
    this->updatePairData(pairData);

    if (MONITOR_PORT == port)
    {
        // handle as network monitoring
        monitor->handleMonitoringPacketBuffer(packet, length, source, port);
    }
    else if (ROUTING_PORT == port || DATA_PORT == port)
    {
        // handle as routing 
        routing->decodeReceivedPacketBuffer(packet, length, source, port);
    }
}

void AdHocRouting::sendPacket(char* packet, int length, IP_ADDR finalDestination, IP_ADDR origIP)
{
    routing->sendPacket(packet, length, finalDestination, origIP);
}

void AdHocRouting::updatePairData(pair_data pairData)
{
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Updating pair data from AdHocRouting" << endl;

    monitor->updatePairData(pairData);

    // after creating new models, update local models in network monitoring 
    // TODO: Is this necessary? Local models will be updated on their own thread. 
    // TODO: It is doing more than battery... why is update local models doing that? 
//    monitor->updateLocalModels();
}

void AdHocRouting::printGlobalVariables()
{
    if (DEBUG)
    {
        cout << "[DEBUG]: ROUTING_PORT: " << ROUTING_PORT << " DATA_PORT: " << DATA_PORT << " MONITOR_PORT: " << MONITOR_PORT 
                << " LOCAL_DATA_UPDATE_RATE_MS: " << LOCAL_DATA_UPDATE_RATE_MS << endl;
    }
}
