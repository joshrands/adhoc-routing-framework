/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADHOC_ROUTING_HELPER_H
#define ADHOC_ROUTING_HELPER_H

#include "ns3/node.h"
#include "ns3/socket.h"

#include "ns3/port.h"
#include "ns3/aodv_sim.h"
#include "ns3/defines.h"
#include "ns3/hello_sim.h"

#include <map>

namespace ns3 {

class PrintPort : public Port{
public:
    PrintPort(int portId):Port(portId){}
    ~PrintPort(){}
    void handlePacket(char* data, int length, IP_ADDR source){
        std::cout << "[PRINT PORT]: Received ";
        for(int i = 0; i < length; i++){
            std::cout << data[i];
        }
        std::cout << " from\n" << getStringFromIp(source) << std::endl;
    }

};

/* ... */
// TODO: Create a helper with ns3 aodv and sim rem 
class AdHocRoutingHelper : public SimAODV
{
public:
    // default constructor initializes everything 
    AdHocRoutingHelper(Ptr<Node> node, IP_ADDR nodeIp);

    static int AdHocSendPacket(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source);
    static void receivePacket (Ptr<Socket> socket);
    static uint32_t getNs3SimulatedTimeMS();
    static double getNs3SimulatedBattery(IP_ADDR nodeIp);
    static void waitSimulatedTimeForHelloMonitor(int DURATION_MS, SimHelloMonitor* waitingHello);

    SimHelloMonitor* helloMonitor;

    // Network monitoring helpers for Rss and Bandwidth 
    map<uint32_t, double> getLinkRssMap() { return m_linkRssDb; } 
    int getAvailableBandwidthBits() { return m_availableBandwidthBits; }
    void setAvailableBandwidthBits(int availableBandwidth) { this->m_availableBandwidthBits = availableBandwidth; }
    void increaseAvailableBandwidthByBits(int numberOfBits) { this->m_availableBandwidthBits += numberOfBits; }

    void updateLinkRss(uint32_t ip, double rss) { m_linkRssDb[ip] = rss; }
    void updateLinkBandwidth(uint32_t bandwidthBytes);

    string getIpAddressStr() { return getStringFromIp(this->ipAddress); }

protected:
    Ptr<Node> m_node;
    int HELLO_INTERVAL_MS = 1000;

    // Link RSS in decibels
    map<uint32_t, double> m_linkRssDb;

    // Local bandwidth calculation over the last second 
    int m_availableBandwidthBits;

    // So nodes can send packets to each other 
    static std::map<IP_ADDR, Ptr<Node>> m_existingNodes;
};

}

#endif /* ADHOC_ROUTING_HELPER_H */

