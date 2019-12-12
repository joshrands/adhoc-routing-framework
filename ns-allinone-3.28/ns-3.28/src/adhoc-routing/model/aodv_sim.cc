#include "aodv_sim.h"

AODVSim::AODVSim()
{

}

AODVSim::AODVSim(IP_ADDR ip) : AODV(ip)
{
    cout << "[INFO]: Creating new simulated aodv" << endl;
}

void AODVSim::getOneHopNeighbors()
{
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Deprecated? Grabbing one hop neighbors from network monitoring" << endl;
}

int AODVSim::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port)
{
    // call other function
    return simSocketSendPacket(buffer, length, dest, port, this->getIp());
}