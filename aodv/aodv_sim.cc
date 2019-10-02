#include "aodv_sim.h"

AODVns3::AODVns3()
{

}

AODVns3::AODVns3(IP_ADDR ip) : AODV(ip)
{
    cout << "Creating new simulated aodv" << endl;
}

int AODVns3::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port)
{
    // call other function
    return ns3SocketSendPacket(buffer, length, dest, port, this->getIp());
}