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

#define TRANS_POWER       10
#define RX_GAIN           10

#define minSpeed_mpers    2
#define maxSpeed_mpers    4
#define xSize_m           300
#define ySize_m           300

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("AODVTest");

void ReceivePacket (Ptr<Socket> socket)
{
  while (socket->Recv ())
    {
      NS_LOG_UNCOND ("Received one packet!");
    }
}

void GenerateTraffic (NodeContainer c, uint32_t pktSize,
                             uint32_t pktCount, Time pktInterval )
{

  if (pktCount > 0)
  {
    Ptr<Socket> socket = Socket::CreateSocket (c.Get (rand()%c.GetN()), UdpSocketFactory::GetTypeId ());
    InetSocketAddress remote = InetSocketAddress (Ipv4Address ("255.255.255.255"), 654);
    socket->SetAllowBroadcast (true);
    socket->Connect (remote);
    socket->Send (Create<Packet> (pktSize));
    socket->Close();
    Simulator::Schedule (pktInterval, &GenerateTraffic,
                         c, pktSize, pktCount - 1, pktInterval);
  }
}

int main (int argc, char *argv[])
{
  std::string phyMode ("DsssRate1Mbps");
  double rss = -80;  // -dBm
  uint16_t numNodes = 5;

  uint32_t packetSize = 20; // bytes
  uint32_t numPackets = 10;
  bool verbose = false;
  double duration = 60.00;

  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("rss", "received signal strength", rss);
//  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
//  cmd.AddValue ("numPackets", "number of packets generated", numPackets);
//  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
 
  cmd.Parse (argc, argv);
  // Convert to time object
  //Time interPacketInterval = Seconds (interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue (phyMode));

  NodeContainer c;
  c.Create (numNodes);

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }
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

/* Keep nodes immobile for initial tests 
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
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("192.168.1.0", "255.255.255.0");
  ipv4.Assign (devices);

  for(int i = 0; i < numNodes; i++){
    Ptr<Socket> recvSink = Socket::CreateSocket (c.Get (i), UdpSocketFactory::GetTypeId ());
    InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 654);
    recvSink->Bind (local);
    recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));
  }

  // Tracing
//  wifiPhy.EnablePcap ("wifi-simple-adhoc", devices);

  Simulator::Schedule (Seconds (1.0), &GenerateTraffic,
                       c, packetSize, numPackets, Seconds(1));

  Simulator::Stop (Seconds (duration + 10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
