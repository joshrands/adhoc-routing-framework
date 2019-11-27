#ifndef AODVSIM_H
#define AODVSIM_H

/*********************************
 * aodv_sim.h
 *
 * Simulated aodv class implementations 
 * 
 * Author: Josh Rands
 * Date: 9/30/2019
 ********************************/

#include "aodv.h"

class AODVns3 : public AODV
{
public:
    // constructors 
    AODVns3();
    AODVns3(IP_ADDR ip);

    // TODO: implement later with network monitoring
	void getOneHopNeighbors();

    int socketSendPacket(char *buffer, int length, IP_ADDR dest, int port);
    function<int(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)> ns3SocketSendPacket;
};

#endif