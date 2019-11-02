#include "hardware_aodv.h"
#include <stdlib.h>
#include <iostream>

//TODO: call AODV constructor on all calls

// Helpers
void HardwareAODV::_hardwareAODV(){
    //TODO: Only set aodv socket to broadcasting
    aodvSocket = new UDPSocket();
    int reuseVal = 1;
    if(!aodvSocket->init()){
        fprintf(stderr, "Could not initialize the aodv socket\n");
    }
    if(!aodvSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &reuseVal, sizeof(reuseVal))){
        fprintf(stderr, "Could not set the aodv socket to reuse ports\n");
    }
    if (!aodvSocket->bindToPort(AODV_PORT)) {
        fprintf(stderr, "Could not bind the aodv socket to port:%d\n", AODV_PORT);
    }
    if(!aodvSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the aodv socket to broadcasting\n");
        exit(-1);
    }

    dataSocket = new UDPSocket();
    if(!aodvSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &reuseVal, sizeof(reuseVal))){
        fprintf(stderr, "Could not set the data socket to reuse ports\n");
    }
    if (!dataSocket->bindToPort(DATA_PORT)) {
        fprintf(stderr, "Could not bind the data socket to port:%d\n", DATA_PORT);
    }
    if(!dataSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the data socket to broadcasting\n");
        exit(-1);
    }
    
    // create threads 
    aodving = thread(&UDPSocket::receiveFromPortThread, aodvSocket);
    dataing = thread(&UDPSocket::receiveFromPortThread, dataSocket);

    aodving.detach();
    dataing.detach();
}

// Constructors
HardwareAODV::HardwareAODV(){
    _hardwareAODV();
}

HardwareAODV::HardwareAODV(uint32_t ip) : AODV(ip) {
    _hardwareAODV();
}

HardwareAODV::HardwareAODV(const char* ip) : AODV(ip) {
    _hardwareAODV();
}

// Destructors
HardwareAODV::~HardwareAODV(){
    aodving.~thread();
    dataing.~thread();
}

// Override functions
int HardwareAODV::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port){
    if(port == AODV_PORT){
        if(H_DEBUG){
            printf("[DEBUG]: sending packet %s through AODV\n", buffer);
        }
        return aodvSocket->sendTo(buffer, length, dest, port);
    }else{
        if(H_DEBUG){
            printf("[DEBUG]: sending packet %s through DATA\n", buffer);
        }
        return dataSocket->sendTo(buffer, length, dest, port);
    }
}

// Class methods
int HardwareAODV::handleAODVPackets(){
    Message message;
    int count = 0;
    if(H_DEBUG){
        printf("[DEBUG]: Handling received packets from AODV\n");
    }
    while(aodvSocket->getMessage(message)){
        printf("Received: %s", message.getData());
        decodeReceivedPacketBuffer(message.getData(), message.getLength(), message.getAddressI());
        count ++;
    }
    if(H_DEBUG){
        printf("[DEBUG]: Handled %d packets from AODV\n", count);
    }
    return (count == 0) ? -1 : count;
}

bool HardwareAODV::getDataPacket(Message& message){
    if(H_DEBUG){
        printf("[DEBUG]: Checking data socket for messages and returning them\n");
    }
    return dataSocket->getMessage(message);
}