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
    UDPSocket* aodvSocket;
    UDPSocket* dataSocket;
    thread aodving;
    thread dataing;

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
    HardwareAODV(uint32_t ip, int aodv_port, int data_port);
    HardwareAODV(const char* ip);

    // Destructors
    ~HardwareAODV();

    // Override functions
    int socketSendPacket(char *buffer, int length, IP_ADDR dest, int port) override;
    
    // Class methods
    
    /*!
     * @brief Reads all messages from sockets and has AODV handle them accordingly
     * 
     * @return -1 if no packets or the number of packets handled
     */
    int handlePackets();

    /*!
     * @brief Returns one of the packets on the data socket
     * 
     * @return vector of received packets
     */
    vector<Message> getDataPackets();
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