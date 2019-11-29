/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adhoc-routing-helper.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/simulator.h"
#include "ns3/inet-socket-address.h"
#include <iostream>

namespace ns3 {

std::map<IP_ADDR, Ptr<Node>> AdHocRoutingHelper::m_existingNodes;

int AdHocRoutingHelper::AdHocSendPacket(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)
{
    Ptr<Node> sourceNode = AdHocRoutingHelper::m_existingNodes[source];
    Ptr<Node> destNode = AdHocRoutingHelper::m_existingNodes[dest];

    if (dest == getIpFromString(BROADCAST_STR))
    {
        // set destination to broadcast  
        Ipv4Address destAddr(getIpFromString(BROADCAST_STR));

        // create socket between nodes 
        Ptr<Socket> socket = Socket::CreateSocket(sourceNode, UdpSocketFactory::GetTypeId());
        InetSocketAddress remote = InetSocketAddress(destAddr, port);
        // allow broadcast 
        socket->SetAllowBroadcast(true);
        // connect the nodes with the new socket  
        socket->Connect(remote); 

        // create packet 
        Ptr<Packet> packet = Create<Packet>(reinterpret_cast<const uint8_t*> (buffer), length); 
        Ipv4Header header;
        // fill header
        header.SetSource(sourceNode->GetObject<Ipv4>()->GetAddress(1,0).GetLocal());
        packet->AddHeader(header);

        socket->Send(packet);
    }
    else 
    {
        // set destination to this node and broadcast
        Ptr<Ipv4> destIpv4 = destNode->GetObject<Ipv4>(); // Get Ipv4 instance of the node
        Ipv4Address destAddr = destIpv4->GetAddress (1, 0).GetLocal();  

        // create socket 
        Ptr<Socket> socket = Socket::CreateSocket(sourceNode, UdpSocketFactory::GetTypeId());
        InetSocketAddress remote = InetSocketAddress(destAddr, port);
        socket->SetAllowBroadcast(false);
        socket->Connect(remote);  

        // create packet 
        Ptr<Packet> packet = Create<Packet>(reinterpret_cast<const uint8_t*> (buffer), length); 
        Ipv4Header header;
        // fill header
        header.SetSource(sourceNode->GetObject<Ipv4>()->GetAddress(1,0).GetLocal());
        packet->AddHeader(header);

        socket->Send(packet);
    }

    return 0;
}

void AdHocRoutingHelper::receivePacket(Ptr<Socket> socket)
{
    Ptr<Packet> packet;
    while (packet = socket->Recv ())
    {
        Ipv4Header header;
        packet->RemoveHeader(header);

        uint32_t length = packet->GetSize();
        uint8_t* packetBuffer = (uint8_t*)(malloc(length));

        packet->CopyData(packetBuffer, length);
        
        Ptr<Ipv4> ipv4 =  socket->GetNode()->GetObject<Ipv4> (); // Get Ipv4 instance of the node

/*        Address address;
        socket->GetPeerName (address);
        InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (address);

        std::cout << "Received one packet!  Socket: " << iaddr.GetIpv4 () << " port: " << iaddr.GetPort ();
        uint16_t port = iaddr.GetPort(); 
*/
        std::cout << "Received data on port " << socket->m_port << std::endl;

        // add aodv object
        Ipv4Address addr = header.GetSource(); 
        uint8_t* ipBuf = (uint8_t*)(malloc(4)); 
        addr.Serialize(ipBuf);

        IP_ADDR source;
        memcpy(&(source),(ipBuf),4);

        std::cout << "Node " << getStringFromIp(socket->GetNode()->m_nodeIp) << " from Node " << getStringFromIp(source) << std::endl;

        pair_data data;
        // TODO: Fill pair data 
        data.pairIp = source; 
        data.rss = -50;

        socket->GetNode()->m_AdHocRoutingHelper->receivePacketWithPairData((char*)(packetBuffer), length, source, socket->m_port, data);
    }
}

uint32_t AdHocRoutingHelper::getNs3SimulatedTimeMS()
{
    return (uint32_t)Simulator::Now().GetMilliSeconds();
}

double AdHocRoutingHelper::getNs3SimulatedBattery(IP_ADDR nodeIp)
{
    return 100;
}

AdHocRoutingHelper::AdHocRoutingHelper(Ptr<Node> node, IP_ADDR nodeIp)
{
    m_node = node;
    m_node->m_AdHocRoutingHelper = this;

    AdHocRoutingHelper::m_existingNodes[nodeIp] = node;

    // create a routing protocol 
    AODVSim* aodv = new AODVSim(nodeIp);
    this->routing = aodv;
    std::cout << "[WARNING]: Must override AODV simSocketSend" << std::endl;
    aodv->simSocketSendPacket = &AdHocSendPacket;
    std::cout << "[INFO]: Override successful." << std::endl;

    // create network monitoring 
    REMSim* rem = new REMSim(nodeIp);
    rem->getSimulatedBatteryLevel = &(AdHocRoutingHelper::getNs3SimulatedBattery);
    rem->getSimulatedTime = &(AdHocRoutingHelper::getNs3SimulatedTimeMS);
    rem->routing = aodv;

    rem->initialize(nodeIp);
    this->monitor = rem;
    std::cout << "[WARNING]: Must override REM getSimulatedBattery and getSimulatedTime" << std::endl;
    std::cout << "[INFO]: Override successful." << std::endl;

    std::cout << "[WARNING]: Must start a recurring event to call monitor->updateLocalModels" << std::endl;

    std::cout << "[WARNING]: Use AdHocRoutingHelper sendPacket and receivePacket functions." << std::endl;
}

}

