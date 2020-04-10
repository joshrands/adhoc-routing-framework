#ifndef HARDWAREAODV_H
#define HARDWAREAODV_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include "endpoint.h"
#include "udp_socket.h"
#include "aodv.h"
#include "hardware_defines.h"

class HardwareAODV : public AODV{
private:
    void _hardwareAODV();
    
protected:
    unordered_map<int, thread> portThreads;
    unordered_map<int, UDPSocket*> portSockets;
    UDPSocket* aodvSocket;
    thread aodving;

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

    virtual int handlePackets() override;

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