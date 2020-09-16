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

// custom ad hoc implementation
#include "ns3/adhoc-routing-helper.h"

#define NS3_DEBUG         1

#define TRANS_POWER       10
#define RX_GAIN           10

#define minSpeed_mpers    3
#define maxSpeed_mpers    5
#define xSize_m           400
#define ySize_m           400
#define LOCAL_MONITOR_INTERVAL  2

#define NUM_NODES         10

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/packet.h"
#include "ns3/basic-energy-source-helper.h"

/**** CSM SmallSat ns3 includes ****/
#include "ns3/routing_protocol.h"
/***********************************/

#include "battery-drain.h"

#include <iostream>

using namespace ns3;

NodeContainer nodes;
map<Ptr<Node>, AdHocRoutingHelper*> adhocMap;
map< IP_ADDR, Ptr<Node> > nodeMap;
EnergySourceContainer energySources;

// Set the socket receive packet callback to the adhoc routing helper 
void ReceiveCallback(Ptr<Socket> socket)
{
    AdHocRoutingHelper::receivePacket(socket);
}

void PrintBandwidth(AdHocRoutingHelper* adhocHelperFrom, AdHocRoutingHelper* adhocHelperTo, int freqMS)
{
  std::cout << "[TEST]" << adhocHelperFrom->getIpAddressStr() << " Available bandwidth = " << adhocHelperFrom->getAvailableBandwidthBits() << std::endl;

  std::cout << "[TEST]" << adhocHelperFrom->getIpAddressStr() << " link bandwidth to " << adhocHelperTo->getIpAddressStr()
		  << " : " << adhocHelperFrom->getLinkBandwidthBits(adhocHelperTo) << std::endl;

  Simulator::Schedule(MilliSeconds(freqMS), &PrintBandwidth, adhocHelperFrom, adhocHelperTo, freqMS);
}

void testAdHoc()
{
  std::cout << "[TEST]: Sending message from node 1 to node 2" << std::endl;
  // Test sending from node 1 to node 2
  string msg = "Hello friend!";
  uint32_t length = msg.length();
  char* buffer = (char*)(malloc(length));
  for (uint32_t i = 0; i < length; i++)
        buffer[i] = msg.at(i);

  auto it = nodeMap.begin();
  // send data from first node to last node 
  AdHocRoutingHelper* adhoc = adhocMap[it->second];
  it++;
  IP_ADDR dest = it->first;
  adhoc->sendPacket(DATA_PORT, buffer, msg.length(), dest); 

  // Print per-link bandwidth for all nodes connected to node 2
  for(uint32_t i = 0; i < nodes.GetN(); i++)
  {
    std::cout << "[TEST]: Per-link bandwidth from " << it->second->m_AdHocRoutingHelper->getIpAddressStr()
        << " to " << nodes.Get(i)->m_AdHocRoutingHelper->getIpAddressStr() << ": "
        << it->second->m_AdHocRoutingHelper->getLinkBandwidthBits(nodes.Get(i)->m_AdHocRoutingHelper)
        << std::endl;
  }

  Simulator::Schedule(Seconds(0.5), &testAdHoc);

  delete buffer;
}

void localMonitoring()
{
  auto it = adhocMap.begin();
  while (it != adhocMap.end())
  {
    it->second->networkMonitor->updateLocalModels();
    it++;
  }

  Simulator::Schedule(Seconds(LOCAL_MONITOR_INTERVAL), &localMonitoring);
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

  Packet::EnableChecking();
  Packet::EnablePrinting();

  nodes.Create (numNodes);

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
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  /* ADD ENERGY MODEL */
  BasicEnergySourceHelper basicSourceHelper;
  // configure energy source
  basicSourceHelper.Set("BasicEnergySourceInitialEnergyJ", DoubleValue(500.0));
  // install source
  energySources = basicSourceHelper.Install(nodes);
  // device energy model
  WifiRadioEnergyModelHelper radioEnergyHelper;
  // configure radio energy model
  radioEnergyHelper.Set("TxCurrentA", DoubleValue(0.0174));
  // install device model
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install(devices, energySources);
  /********************/

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

  // Keep nodes immobile for initial tests 
  std::string speedInput = "ns3::UniformRandomVariable[Min=" + std::to_string(minSpeed_mpers) + "|Max=" + std::to_string(maxSpeed_mpers) + "]";
  // set mobility model for adhoc nodes
  mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel",
    "Speed", StringValue(speedInput),
    "Pause", StringValue("ns3::ConstantRandomVariable[Constant=0.5]"),
    "PositionAllocator", PointerValue(positionAllocPtr));

  mobility.Install(nodes);

  InternetStackHelper internet;
  internet.Install(nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("192.168.1.0", "255.255.255.0");
  ipv4.Assign (devices);

  vector<Vector> currentPositions;
  for(int i = 0; i < numNodes; i++){
    Ptr<Socket> routeSink = Socket::CreateSocket (nodes.Get (i), UdpSocketFactory::GetTypeId ());
    InetSocketAddress localRouting = InetSocketAddress (Ipv4Address::GetAny (), ROUTING_PORT);
    routeSink->Bind(localRouting);
    routeSink->SetRecvCallback(MakeCallback(&ReceiveCallback));
    routeSink->m_port = ROUTING_PORT;

    Ptr<Socket> dataSink = Socket::CreateSocket(nodes.Get(i), UdpSocketFactory::GetTypeId()); 
    InetSocketAddress localData = InetSocketAddress (Ipv4Address::GetAny (), DATA_PORT);
    dataSink->Bind(localData);
    dataSink->SetRecvCallback(MakeCallback(&ReceiveCallback));
    dataSink->m_port = DATA_PORT;

    Ptr<Socket> monitorSink = Socket::CreateSocket(nodes.Get(i), UdpSocketFactory::GetTypeId());
    InetSocketAddress localMonitor = InetSocketAddress (Ipv4Address::GetAny (), MONITOR_PORT);
    monitorSink->Bind(localMonitor);
    monitorSink->SetRecvCallback(MakeCallback(&ReceiveCallback));
    monitorSink->m_port = MONITOR_PORT;

    Ptr<Socket> helloSink = Socket::CreateSocket(nodes.Get(i), UdpSocketFactory::GetTypeId());
    InetSocketAddress localHello = InetSocketAddress (Ipv4Address::GetAny (), HELLO_PORT);
    helloSink->Bind(localHello);
    helloSink->SetRecvCallback(MakeCallback(&ReceiveCallback));
    helloSink->m_port = HELLO_PORT;

    Ptr<Ipv4> ipv4 = nodes.Get(i)->GetObject<Ipv4>(); // Get Ipv4 instance of the node
    Ipv4Address addr = ipv4->GetAddress (1, 0).GetLocal ();  
    // add aodv object 
    uint8_t* ipBuf = (uint8_t*)(malloc(4)); 
    addr.Serialize(ipBuf);

    IP_ADDR ip;
    memcpy(&(ip),(ipBuf),4);

    nodes.Get(i)->m_nodeIp = ip;
    nodeMap[ip] = nodes.Get(i); 
 
    adhocMap[nodes.Get(i)] = new AdHocRoutingHelper(nodes.Get(i), ip);
    nodes.Get(i)->m_AdHocRoutingHelper = adhocMap[nodes.Get(i)]; 
    nodes.Get(i)->m_AdHocRoutingHelper->setAvailableBandwidthBits(BANDWIDTH_BITS_MAX);

//    Simulator::Schedule(Seconds(1 + i), &PrintBandwidth, nodes.Get(i)->m_AdHocRoutingHelper, 10);

    Vector pos = nodes.Get(i)->GetObject<RandomWaypointMobilityModel>()->GetPosition();
    if (DEBUG)
      cout << "[DEBUG]: Node " << i << " at " << pos.x << ", " << pos.y << endl;
    currentPositions.push_back(pos);

    Simulator::Schedule(MilliSeconds(1001 + i), &AdHocRoutingHelper::waitSimulatedTimeForHelloMonitor, 2000, adhocMap[nodes.Get(i)]->helloMonitor);
  }

  Simulator::Schedule(Seconds(10.0), &testAdHoc);
  // TODO: Add network monitroing back.
  Simulator::Schedule(Seconds(LOCAL_MONITOR_INTERVAL), &localMonitoring);
  // Print bandwidth
  Simulator::Schedule(Seconds(1), &PrintBandwidth, nodes.Get(0)->m_AdHocRoutingHelper, nodes.Get(1)->m_AdHocRoutingHelper, 10);
  // Drain battery
//  Simulator::Schedule(Seconds(0.1), &DrainBatteryMobile, nodes, energySources, currentPositions);

  Simulator::Stop (Seconds (duration + 10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
