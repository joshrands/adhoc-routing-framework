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

#include "../aodv/aodv.h"

class AODVSim : public AODV
{
public:
    // constructors 
    AODVSim();
    AODVSim(IP_ADDR ip);

    // TODO: implement later with network monitoring
	void getOneHopNeighbors();

    int socketSendPacket(char *buffer, int length, IP_ADDR dest, int port);
    function<int(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)> simSocketSendPacket;
};

#endif