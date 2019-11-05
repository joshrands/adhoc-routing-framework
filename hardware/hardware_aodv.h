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
     * @brief Reads all messages from aodv socket and handles them accordingly
     * 
     * @return -1 if no packets or the number of packets handled
     */
    int handleAODVPackets();

    /*!
     * @brief Returns one of the packets on the data socket
     * 
     * @param message the first message on the queue
     * @return true a message was received
     * @return false no messages
     */
    bool getDataPacket(Message& message);
};


#endif