#include "aodv_sim.h"

AODVns3::AODVns3()
{

}

AODVns3::AODVns3(IP_ADDR ip) : AODV(ip)
{
    cout << "[INFO]: Creating new simulated aodv" << endl;
}

void AODVns3::getOneHopNeighbors()
{
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Grabbing one hop neighbors from network monitoring" << endl;
}

int AODVns3::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port)
{
    // call other function
    return ns3SocketSendPacket(buffer, length, dest, port, this->getIp());
}