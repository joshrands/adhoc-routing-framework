/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Boeing Company
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  */

// custom aodv implementation
#include "ns3/aodv_sim.h"

#define NS3_DEBUG         1

#define TRANS_POWER       10
#define RX_GAIN           10

#define minSpeed_mpers    2
#define maxSpeed_mpers    4
#define xSize_m           500
#define ySize_m           500

#define NUM_NODES         10

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/packet.h"

#include <iostream>

using namespace ns3;

NodeContainer c;
AODVns3* aodvArray[NUM_NODES];
map< IP_ADDR, Ptr<Node> > nodeMap;
map< Ptr<Node>, AODVns3* > aodvMap;

void testAodv()
{
  // Test sending from node 1 to node 3
  string msg = "Hello world";
  uint32_t length = msg.length();
  char* buffer = (char*)(malloc(length));
  for (uint32_t i = 0; i < length; i++)
    buffer[i] = msg.at(i);

  aodvArray[rand() % NUM_NODES]->sendPacket(buffer, msg.length(), aodvArray[rand() % NUM_NODES]->getIp());

  Simulator::Schedule(Seconds(1.0), &testAodv);

  delete buffer;
}

int SendPacket(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)
{
  if (NS3_DEBUG)
    cout << "Sending a packet from " << getStringFromIp(source) << " to " << getStringFromIp(dest) << endl;

  Ptr<Node> sourceNode = nodeMap[source];
  Ptr<Node> destNode = nodeMap[dest];

  if (dest == getIpFromString(BROADCAST))
  {
    // set destination to broadcast  
    Ipv4Address destAddr(getIpFromString(BROADCAST));

    // create socket between nodes 
    Ptr<Socket> socket = Socket::CreateSocket(sourceNode, UdpSocketFactory::GetTypeId());
    InetSocketAddress remote = InetSocketAddress(destAddr, AODVns3::AODV_PORT);
    // allow broadcast 
    socket->SetAllowBroadcast(true);
    socket->Connect(remote); 

    // create packet 
    Ptr<Packet> packet = Create<Packet>(reinterpret_cast<const uint8_t*> (buffer), length); 
    Ipv4Header header;
    // fill header
    header.SetSource(sourceNode->GetObject<Ipv4>()->GetAddress(1,0).GetLocal());
    packet->AddHeader(header);

    if (NS3_DEBUG)
      cout << "Broadcasting packet from Node " << sourceNode->GetId() << endl;

    socket->Send(packet);
  }
  else 
  {
    // set destination to this node and broadcast
    Ptr<Ipv4> destIpv4 = destNode->GetObject<Ipv4>(); // Get Ipv4 instance of the node
    Ipv4Address destAddr = destIpv4->GetAddress (1, 0).GetLocal();  

    // create socket 
    Ptr<Socket> socket = Socket::CreateSocket(sourceNode, UdpSocketFactory::GetTypeId());
    InetSocketAddress remote = InetSocketAddress(destAddr, AODVns3::AODV_PORT);
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

void ReceivePacket (Ptr<Socket> socket)
{
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

      aodvMap[socket->GetNode()]->decodeReceivedPacketBuffer((char*)(packetBuffer), length, source);
    }
}


int main (int argc, char *argv[])
{
  std::string phyMode ("DsssRate1Mbps");
  double rss = -80;  // -dBm
  uint16_t numNodes = NUM_NODES;

  double duration = 60.00;

  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("rss", "received signal strength", rss);
 
  cmd.Parse (argc, argv);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue (phyMode));

  c.Create (numNodes);

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // This is one parameter that matters when using FixedRssLossModel
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (RX_GAIN) );
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  // The below FixedRssLossModel will cause the rss to be fixed regardless
  // of the distance between the two stations, and the transmit power
  wifiChannel.AddPropagationLoss("ns3::LogDistancePropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  wifiPhy.Set("TxPowerStart", DoubleValue(TRANS_POWER));
  wifiPhy.Set("TxPowerEnd", DoubleValue(TRANS_POWER));

  // Add a mac and disable rate control
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);

  // Note that with FixedRssLossModel, the positions below are not
  // used for received signal strength.
  MobilityHelper mobility;

  std::string xBounds = "ns3::UniformRandomVariable[Min=" + std::to_string(-1 * xSize_m/2)
    + "|Max=" + std::to_string(xSize_m/2) + "]";
  std::string yBounds = "ns3::UniformRandomVariable[Min=" + std::to_string(-1 * ySize_m/2)
    + "|Max=" + std::to_string(ySize_m/2) + "]";

  ObjectFactory positionModel;
  positionModel.SetTypeId("ns3::RandomRectanglePositionAllocator");
  positionModel.Set("X", StringValue(xBounds));
  positionModel.Set("Y", StringValue(yBounds));
  Ptr<PositionAllocator> positionAllocPtr = positionModel.Create()->GetObject<PositionAllocator>();
  mobility.SetPositionAllocator(positionAllocPtr);

/*  // Keep nodes immobile for initial tests 
  std::string speedInput = "ns3::UniformRandomVariable[Min=" + std::to_string(minSpeed_mpers) + "|Max=" + std::to_string(maxSpeed_mpers) + "]";
  // set mobility model for adhoc nodes
  mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel",
    "Speed", StringValue(speedInput),
    "Pause", StringValue("ns3::ConstantRandomVariable[Constant=0.5]"),
    "PositionAllocator", PointerValue(positionAllocPtr));
*/

  mobility.Install (c);

  InternetStackHelper internet;
  internet.Install (c);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("192.168.1.0", "255.255.255.0");
  ipv4.Assign (devices);

  for(int i = 0; i < numNodes; i++){
    Ptr<Socket> recvSink = Socket::CreateSocket (c.Get (i), UdpSocketFactory::GetTypeId ());
    InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), AODVns3::AODV_PORT);
    recvSink->Bind (local);
    recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

    Ptr<Ipv4> ipv4 = c.Get(i)->GetObject<Ipv4> (); // Get Ipv4 instance of the node
    Ipv4Address addr = ipv4->GetAddress (1, 0).GetLocal ();  
    // add aodv object 
    uint8_t* ipBuf = (uint8_t*)(malloc(4)); 
    addr.Serialize(ipBuf);

    IP_ADDR ip;
    memcpy(&(ip),(ipBuf),4);

    aodvArray[i] = new AODVns3(ip);
    aodvArray[i]->ns3SocketSendPacket = &SendPacket;

    nodeMap[ip] = c.Get(i);
    aodvMap[c.Get(i)] = aodvArray[i];
  }

  // Tracing
//  wifiPhy.EnablePcap ("wifi-simple-adhoc", devices);

//  Simulator::Schedule (Seconds (1.0), &GenerateTraffic,
//                       c, packetSize, numPackets, Seconds(1));

  Simulator::Schedule(Seconds(1.0), &testAodv);// &(aodvArray[1]->socketSendPacket), buffer, msg.length() + 5, aodvArray[1]->getIp(), 654, aodvArray[3]->getIp());

  Simulator::Stop (Seconds (duration + 10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
