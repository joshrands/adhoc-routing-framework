#ifndef HARDWAREAODV_H
#define HARDWAREAODV_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include "../socket/endpoint.h"
#include "../socket/udp_socket.h"
#include "../aodv/aodv.h"
#include "hardware_defines.h"

class HardwareAODV : public AODV{
private:
    vector<thread> portThreads;
    unordered_map<int, UDPSocket*> portSockets;
    UDPSocket* aodvSocket;
    thread aodving;

    void _hardwareAODV();
public:
    // Constructors

    HardwareAODV();

    /*!
     * @brief Construct a new HardwareAODV object
     * 
     * @param ip the ip address of the current device
     */
    HardwareAODV(uint32_t ip);
    HardwareAODV(const char* ip);

    // Destructors
    ~HardwareAODV();

    int handlePackets() override;
	
	// Network Monitoring
	// attempt to repair the link and then send the packet to its destination
	//virtual void repairLink(int port, IP_ADDR brokenLink, IP_ADDR finalDest, char* buffer, int length, IP_ADDR origIP);
	// attempt to repair the link
	//virtual bool attemptLocalRepair(IP_ADDR brokenLink, IP_ADDR finalDest);
	// get the next door neighbors in the network
	//virtual void getOneHopNeighbors();

protected:
    bool _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) override;
	void _buildPort(Port*) override;
    void _destroyPort(Port*) override;
};

/*!
 * @brief prints the data of a packet to the specified filed
 * 
 * @param file  where to print the packet
 * @param buffer the packet to print
 * @param length the length of the packet
 */
void printPacket(FILE* file, char * buffer, int length);


#endif