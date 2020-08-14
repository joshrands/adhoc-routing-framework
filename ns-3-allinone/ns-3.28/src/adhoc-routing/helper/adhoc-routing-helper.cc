/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adhoc-routing-helper.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/simulator.h"
#include "ns3/inet-socket-address.h"
#include "ns3/tag.h"
#include "ns3/udp-header.h"

#include "ns3/network_monitor.h"
#include "ns3/aodv_sim.h"
#include "ns3/rem_sim.h"
#include "ns3/port.h"

#include <iostream>

namespace ns3 {

std::map<IP_ADDR, Ptr<Node>> AdHocRoutingHelper::m_existingNodes;

int AdHocRoutingHelper::AdHocSendPacket(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)
{
    if (DEBUG) 
        std::cout << "[ADHOC_HELPER]:[DEBUG]: Source node = " << getStringFromIp(source) 
                  << " to destination node = " << getStringFromIp(dest) << std::endl;

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

        return socket->Send(packet);
    }
    else 
    {
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

        return socket->Send(packet);
    }
}

void AdHocRoutingHelper::receivePacket(Ptr<Socket> socket)
{
    Ptr<Packet> packet;

    // Add this packet to the buffer of current packets and make callback to remove it in 1 second 
    int packetSize = socket->GetRxAvailable();

    while (packet = socket->Recv ())
    {
        Ipv4Header header;
        packet->RemoveHeader(header); 

        // get rss data from packet 
        WifiTag tag;
        packet->PeekPacketTag(tag);

        uint32_t length = packet->GetSize();
        uint8_t* packetBuffer = (uint8_t*)(malloc(length));

        packet->CopyData(packetBuffer, length);
        
        Ptr<Ipv4> ipv4 =  socket->GetNode()->GetObject<Ipv4> (); // Get Ipv4 instance of the node

        if (DEBUG)
            std::cout << "[DEBUG]: Received data on port " << socket->m_port << std::endl;

        // add aodv object
        Ipv4Address addr = header.GetSource(); 
        uint8_t* ipBuf = (uint8_t*)(malloc(4)); 
        addr.Serialize(ipBuf);

        IP_ADDR source;
        memcpy(&(source),(ipBuf),4);

        if (DEBUG)
            std::cout << "[DEBUG]: Node " << getStringFromIp(socket->GetNode()->m_nodeIp) << " from Node " << getStringFromIp(source) << std::endl;

        pair_data packetPairData;
        packetPairData.pairIp = source; 
        packetPairData.rss = tag.GetRssValue();

        if (DEBUG)
            std::cout << "[DEBUG]: Received RSS: " << packetPairData.rss << std::endl;

        // Add the packet to this nodes buffer 
        SimPacket packet;
        packet.data = (char*)packetBuffer;
        packet.length = length;
        packet.packetPairData = packetPairData;
        packet.portId = socket->m_port;
        packet.source = source;

        AdHocRoutingHelper* adhocRoutingHelper = socket->GetNode()->m_AdHocRoutingHelper;

        // Log this interaction as a 'hello' message
        adhocRoutingHelper->helloMonitor->receiveHelloMessage(source);

        // Update network statistics
        // Update rss 
        adhocRoutingHelper->updateLinkRss(source, packetPairData.rss);
        // Update bandwidth 
        adhocRoutingHelper->updateLinkBandwidth(packetSize); 

        // Push the packet onto the packet queue and then handle it
        adhocRoutingHelper->packetQueue.push(packet);
        adhocRoutingHelper->handlePackets();
    }
}


int AdHocRoutingHelper::getLinkBandwidthBits(IP_ADDR linkIp)
{
	// Determine if we have a direct link to linkIp Node
	if(!this->directLink(linkIp))
	{
		// No direct link, link bandwidth is 0
		return 0;
	}
	// Return available Bandwidth TODO: to free BW + used
	return m_availableBandwidthBits;
}

void RemovePacketFromBandwidthMetric(AdHocRoutingHelper* adhocRoutingHelper, int numberOfBits)
{
    adhocRoutingHelper->increaseAvailableBandwidthByBits(numberOfBits);
}

void AdHocRoutingHelper::updateLinkBandwidth(uint32_t bandwidthBytes)
{
    // Add this packet to current bandwidth and remove it after a second 
    int numberOfBits = bandwidthBytes * 8;
    this->m_availableBandwidthBits -= numberOfBits; 

    if (signed(this->m_availableBandwidthBits) < 0)
    {
        int difference = abs(signed(this->m_availableBandwidthBits));
        this->m_availableBandwidthBits += difference;
        numberOfBits -= difference;
    }

    // since our metric is in Mbps, we will remove these bits in 1 second 
    Simulator::Schedule(Seconds(1.0), &RemovePacketFromBandwidthMetric, this, numberOfBits);
}

uint64_t AdHocRoutingHelper::getNs3SimulatedTimeMS()
{
    return (uint32_t)Simulator::Now().GetMilliSeconds();
}

double AdHocRoutingHelper::getNs3SimulatedBattery(IP_ADDR nodeIp)
{
    // get the battery level of node ip
    Ptr<Node> node = m_existingNodes[nodeIp];
    return node->m_battery;
}

void AdHocRoutingHelper::waitSimulatedTimeForHelloMonitor(int DURATION_MS, SimHelloMonitor* waitingHello)
{
    // duration of 1 so we do 1 iteration and then stop
    // we do this because in a discrete event simulator we can't keep track of time easily
    // instead we schedule sends every DURATION_MS
    waitingHello->sendHellos(1);

    Simulator::Schedule(MilliSeconds(DURATION_MS), &waitSimulatedTimeForHelloMonitor, DURATION_MS, waitingHello);
}

void dumbySleep(int DURATION_MS)
{
}

AdHocRoutingHelper::AdHocRoutingHelper(Ptr<Node> node, IP_ADDR nodeIp) : SimAODV(nodeIp)
{
    m_node = node;
    m_node->m_AdHocRoutingHelper = this;

    AdHocRoutingHelper::m_existingNodes[nodeIp] = node;
    // Override simSocketSendPacket to use ns3 sockets
    this->simSocketSendPacket = &AdHocSendPacket;
    std::cout << "[ADHOC_HELPER]:[INFO]: Using ns3 simSocketSendPacket" << std::endl;

    // create network monitoring
    // NOTE: This is for battery level ONLY right now 
    REMSim* rem = new REMSim(nodeIp);
    rem->getSimulatedBatteryLevel = &(AdHocRoutingHelper::getNs3SimulatedBattery);
    rem->getSimulatedTime = &(AdHocRoutingHelper::getNs3SimulatedTimeMS);
    rem->routing = this;

    rem->initialize(nodeIp);
    // assign SimAODV networkMonitor to this rem
    this->networkMonitor = rem;
    rem->setPortId(MONITOR_PORT);

    // add port to AdHocRoutingHelper
    std::cout << "[ADHOC_HELPER]:[INFO]: Added REM port" << std::endl;
    this->addPort(rem);

    // create hello monitoring 
    SimHelloMonitor* hello = new SimHelloMonitor(HELLO_PORT, this);
    this->helloMonitor = hello;
    // because this is a discrete event simulator, actually waiting doesn't make sense
    // we need to schedule
    hello->waitSimulatedTime = &(dumbySleep);
    hello->getSimulatedTime = &(AdHocRoutingHelper::getNs3SimulatedTimeMS);

    // add port to AdHocRoutingHelper
    std::cout << "[ADHOC_HELPER]:[INFO]: Added hello monitor port" << std::endl;
    this->addPort(hello);

    // create print packet port 
    PrintPort* printPort = new PrintPort(DATA_PORT);
    this->addPort(printPort);
    std::cout << "[ADHOC_HELPER]:[INFO]: Added print port" << std::endl;

    std::cout << "[WARNING]: Must start a recurring event to call monitor->updateLocalModels" << std::endl;
    std::cout << "[WARNING]: Use AdHocRoutingHelper sendPacket and receivePacket functions." << std::endl;
}

// close ns3 namespace
}
