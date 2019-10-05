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

#define AODV_PORT 8060
#define DATA_PORT 8080

class HardwareAODV : public AODV{
private:
    UDPSocket* aodvSocket;
    UDPSocket* dataSocket;
    thread aodving;
    thread dataing;
private:
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

    // Override functions
    int socketSendPacket(char *buffer, int length, IP_ADDR dest, int port) override;
    
    // Class methods
    
    /*!
     * @brief Reads all messages from aodv socket and handles them accordingly
     * 
     */
    void handleReceivedPackets();

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