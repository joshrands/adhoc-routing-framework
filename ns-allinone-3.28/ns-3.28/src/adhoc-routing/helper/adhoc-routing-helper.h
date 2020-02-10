/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADHOC_ROUTING_HELPER_H
#define ADHOC_ROUTING_HELPER_H

#include "ns3/aodv_sim.h"
#include "ns3/defines.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include <map>

namespace ns3 {

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

protected:
    Ptr<Node> m_node;

    static std::map<IP_ADDR, Ptr<Node>> m_existingNodes;
//    static std::map<Ptr<Node>, Ptr<EnergySource>> m_batteryLevels;
};

}

#endif /* ADHOC_ROUTING_HELPER_H */

