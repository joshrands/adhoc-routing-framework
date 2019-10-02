/*********************************
 * aodv_sim.h
 *
 * Simulated aodv class implementations 
 * 
 * Author: Josh Rands
 * Date: 9/30/2019
 ********************************/

#include "aodv.h"

class AODVns3 : AODV
{
public:
    // constructors 
    AODVns3();
    AODVns3(IP_ADDR ip);

    int socketSendPacket(char *buffer, int length, IP_ADDR dest, int port);
    int ns3SocketSendPacket(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source);
//	std::function<int(char* buffer, int length, IP_ADDR dest, int port)> socketSendPacket;

};
