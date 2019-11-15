//#include "aodv/RoutingProtocol.h"
//#include "monitoring/network_monitor.h"
#include "aodv/aodv.h"
#include "monitoring/rem.h"

// class to combine routing and monitoring 
class AdHocRouting
{
public:
    void receivePacketWithPairData(char* packet, int length, IP_ADDR source, int port, pair_data pairData);
 	void sendPacket(char* packet, int length, IP_ADDR finalDestination, IP_ADDR origIP = -1);

    // SHOULD BE FOR TESTING ONLY 
    void updatePairData(pair_data pairData);

    RoutingProtocol* routing;
    NetworkMonitor* monitor;
};
