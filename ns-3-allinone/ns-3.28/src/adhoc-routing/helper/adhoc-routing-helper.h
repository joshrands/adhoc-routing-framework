/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADHOC_ROUTING_HELPER_H
#define ADHOC_ROUTING_HELPER_H

#include "ns3/node.h"
#include "ns3/socket.h"

#include "ns3/port.h"
#include "ns3/aodv_sim.h"
#include "ns3/defines.h"
#include "ns3/hello_sim.h"
#include "ns3/mobility-model.h"

#include <map>
#include <math.h>

#define PRINT_PACKETS       0

namespace ns3 {

class PrintPort : public Port{
public:
    PrintPort(int portId):Port(portId){}
    ~PrintPort(){}
    void handlePacket(char* data, int length, IP_ADDR source){
        if (PRINT_PACKETS){
            std::cout << "[PRINT PORT]: Received ";
            for(int i = 0; i < length; i++){
                std::cout << data[i];
            }
            std::cout << " from\n" << getStringFromIp(source) << std::endl;
        }
    }

};

/* ... */
class AdHocRoutingHelper : public SimAODV
{
public:
    // default constructor initializes everything 
    AdHocRoutingHelper(Ptr<Node> node, IP_ADDR nodeIp);

    static int AdHocSendPacket(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source);
    static void receivePacket (Ptr<Socket> socket);
    static uint64_t getNs3SimulatedTimeMS();
    static double getNs3SimulatedBattery(IP_ADDR nodeIp);
    static void waitSimulatedTimeForHelloMonitor(int DURATION_MS, SimHelloMonitor* waitingHello);

    SimHelloMonitor* helloMonitor;

    // Network monitoring helpers for Rss and Bandwidth 
    map<uint32_t, double> getLinkRssMap() { return m_linkRssDb; } 
    // Returns the available bandwidth based on bandwidth share and distance
    int getLinkBandwidthBits(AdHocRoutingHelper* targetHelper);
    // Returns the number of bits sent to destIP over the last second
    int getLinkBitsSent(IP_ADDR destIP);
    void increaseAvailableBandwidthByBits(IP_ADDR linkIP, int numberOfBits);

    void updateLinkRss(uint32_t ip, double rss) { m_linkRssDb[ip] = rss; }
    void updateLinkBandwidth(IP_ADDR linkIP, uint32_t bandwidthBytes);

    string getIpAddressStr() { return getStringFromIp(this->ipAddress); }
    Ptr<Node> getNode();

protected:
    Ptr<Node> m_node;

    // Link RSS in decibels
    map<uint32_t, double> m_linkRssDb;
	// Holds how much bandwidth is used by each node over the last minute
	std::map<IP_ADDR, int> m_bandwidthUsedMap;

    // So nodes can send packets to each other 
    static std::map<IP_ADDR, Ptr<Node>> m_existingNodes;
};

}

#endif /* ADHOC_ROUTING_HELPER_H */

