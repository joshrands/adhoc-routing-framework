/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adhoc-routing-helper.h"
#include "ns3/socket.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include <iostream>

namespace ns3 {

int AdHocSendPacket(Ptr<Node> sourceNode, Ptr<Node> destNode, 
                    char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)
{
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
void AdHocReceivePacket (Ptr<Socket> socket)
{
/*
  Ptr<Packet> packet;
  while (packet = socket->Recv ())
    {
      NS_LOG_UNCOND ("Received a packet on Node " + to_string(socket->GetNode()->GetId()));
      Ipv4Header header;
      packet->RemoveHeader(header);

      if (NS3_DEBUG)
        cout << "Source = " << header.GetSource() << endl;

      uint32_t length = packet->GetSize();
      uint8_t* packetBuffer = (uint8_t*)(malloc(length));

      packet->CopyData(packetBuffer, length);
      
      Ptr<Ipv4> ipv4 =  socket->GetNode()->GetObject<Ipv4> (); // Get Ipv4 instance of the node
      Ipv4Address addr = header.GetSource();// ipv4->GetAddress (1, 0).GetLocal ();  
      
      // add aodv object 
      uint8_t* ipBuf = (uint8_t*)(malloc(4)); 
      addr.Serialize(ipBuf);

      IP_ADDR source;
      memcpy(&(source),(ipBuf),4);

      //aodvMap[socket->GetNode()]->decodeReceivedPacketBuffer((char*)(packetBuffer), length, source);
    }
*/
}

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

