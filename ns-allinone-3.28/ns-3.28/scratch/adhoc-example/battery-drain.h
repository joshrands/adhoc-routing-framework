#include <iostream>
#include "ns3/vector.h"
#include "ns3/random-waypoint-mobility-model.h"
#include "ns3/basic-energy-source.h"

using namespace std;
using namespace ns3;

#define NS3_BATTERY_DEBUG   0

void DrainBatteryMobile(NodeContainer nodes, EnergySourceContainer energySources);

// drain battery based off mobility of every node
void DrainBatteryMobile(NodeContainer nodes, EnergySourceContainer energySources, vector<Vector> prevPositions)
{
  //cout << "Draining node battery" << endl;

  vector<Vector> currentPositions;

  for (uint16_t i = 0; i < nodes.GetN(); i++)
  {
    Vector pos = nodes.Get(i)->GetObject<RandomWaypointMobilityModel>()->GetPosition();
    double distance = CalculateDistance(pos, prevPositions.at(i));

    Ptr<BasicEnergySource> energy = DynamicCast<BasicEnergySource>(energySources.Get(i));
    energy->SetRemainingEnergy(energy->GetRemainingEnergy() - distance*0.01);

    if (NS3_BATTERY_DEBUG)
      cout << "[BATTERY]: Node " << to_string(i) << " battery level at " << to_string(energy->GetRemainingEnergy()) << endl;

    // set node's battery
    nodes.Get(i)->m_battery = energy->GetRemainingEnergy();

    currentPositions.push_back(pos);
  }

  Simulator::Schedule(Seconds(0.1), &DrainBatteryMobile, nodes, energySources, currentPositions);
}