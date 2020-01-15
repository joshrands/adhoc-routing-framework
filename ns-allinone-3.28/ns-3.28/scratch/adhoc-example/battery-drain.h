#include <iostream>
#include "ns3/vector.h"
#include "ns3/random-waypoint-mobility-model.h"
#include "ns3/basic-energy-source.h"

using namespace std;
using namespace ns3;

void DrainBatteryMobile(NodeContainer nodes, EnergySourceContainer energySources);

// drain battery based off mobility of every node
void DrainBatteryMobile(NodeContainer nodes, EnergySourceContainer energySources, vector<Vector> prevPositions)
{
  //cout << "Draining node battery" << endl;

  vector<Vector> currentPositions;

  for (uint16_t i = 0; i < nodes.GetN(); i++)
  {
    Vector pos = nodes.Get(i)->GetObject<RandomWaypointMobilityModel>()->GetPosition();
//    cout << pos.x << ", " << pos.y << ", " << pos.z << endl;
    double distance = CalculateDistance(pos, prevPositions.at(i));
//    cout << distance << endl;

    Ptr<BasicEnergySource> energy = DynamicCast<BasicEnergySource>(energySources.Get(i));
    energy->SetRemainingEnergy(energy->GetRemainingEnergy() - distance*0.01);

    currentPositions.push_back(pos);
  }

  Simulator::Schedule(Seconds(0.1), &DrainBatteryMobile, nodes, energySources, currentPositions);
}