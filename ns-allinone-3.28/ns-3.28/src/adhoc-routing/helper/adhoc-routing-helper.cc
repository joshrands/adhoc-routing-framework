/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adhoc-routing-helper.h"
#include <iostream>

namespace ns3 {

AdHocRoutingHelper::AdHocRoutingHelper(Ptr<Node> node, IP_ADDR nodeIp)
{
    m_node = node;

    // create a routing protocol 
    AODVSim* aodv = new AODVSim(nodeIp);
    this->routing = aodv;
    std::cout << "[WARNING]: Must override AODV simSocketSend" << std::endl;

    // create network monitoring 
    REMSim* rem = new REMSim();
    rem->initialize(nodeIp);
    rem->routing = aodv;
    this->monitor = rem;
    std::cout << "[WARNING]: Must override REM getSimulatedBattery and getSimulatedTime" << std::endl;

    std::cout << "[WARNING]: Use AdHocRoutingHelper send and receive functions." << std::endl;
}

}

