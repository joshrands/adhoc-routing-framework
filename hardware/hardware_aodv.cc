#include "hardware_aodv.h"
#include <stdlib.h>
#include <iostream>

// Helpers
void HardwareAODV::_hardwareAODV(){
    aodvSocket = new UDPSocket();
    const int REUSE_VAL = 1;
    if(!aodvSocket->init()){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not initialize the aodv socket\n");
    }
    if(!aodvSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &REUSE_VAL, sizeof(REUSE_VAL))){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not set the aodv socket to reuse ports\n");
    }
    if (!aodvSocket->bindToPort(ROUTING_PORT)) {
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not bind the aodv socket to port:%d\n", ROUTING_PORT);
    }
    if(!aodvSocket->setBroadcasting()){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not set the aodv socket to broadcasting\n");
        exit(-1);
    }

    // create thread
    aodving = thread(&UDPSocket::receiveFromPortThread, aodvSocket);
    aodving.detach();
}

// Constructors
HardwareAODV::HardwareAODV():AODV(){
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
    if(HARDWARE_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Destructing hardware aodv\n");
    }
    
    delete aodvSocket;

    int n = portSockets.size();
    while(n-- > 0){
        delete portSockets.begin()->second;
        portSockets.erase(portSockets.begin());
    }
}

// Public functions
int HardwareAODV::handlePackets(){
    int count = 0;
    Message message;
    // Handle aodv packets
    while(aodvSocket->getMessage(message)){
        _handleAODVPacket(message.getData(), message.getLength(), message.getAddressI());
        count ++;
    }
    // Handle packets on the ports
    for(auto socketPair : portSockets){
        while(socketPair.second->getMessage(message)){
            ports[socketPair.first]->handlePacket(message.getData(), message.getLength(), message.getAddressI());
            count++;
        }
    }
    return count;
}

// Private Functions
bool HardwareAODV::_socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest){
    if(portId == ROUTING_PORT){
        if(HARDWARE_DEBUG){
            printf("[HARDWARE]:[DEBUG]: sending packet '");
            printPacket(stdout, buffer, length);
            printf("' through AODV\n");
        }
        return aodvSocket->sendTo(buffer, length, dest, portId) > 0;
    }
    if(ports.count(portId)){
        if(HARDWARE_DEBUG){
            printf("[HARDWARE]:[DEBUG]: sending packet '");
            printPacket(stdout, buffer, length);
            printf("' through port %d\n", portId);
        }
        return portSockets[portId]->sendTo(buffer, length, dest, portId) > 0;
    }
    fprintf(stderr, "[HARDWARE]:[ERROR]: Tried to send packet '");
    printPacket(stderr, buffer, length);
    fprintf(stderr, "' through non-existant port %d\n", portId);
    return false;
}

void HardwareAODV::_buildPort(Port* p){
    // Create socket
    UDPSocket* portSocket = new UDPSocket();
    const int REUSE_VAL = 1;
    if(!portSocket->init()){
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not initialize the aodv socket\n");
    }
    if(!portSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &REUSE_VAL, sizeof(REUSE_VAL))){
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not set the aodv socket to reuse ports\n");
    }
    if (!portSocket->bindToPort(p->getPortId())) {
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not bind the aodv socket to port:%d\n", p->getPortId());
    }
    portSockets[p->getPortId()] = portSocket;

    // create thread
    portThreads.push_back(thread(&UDPSocket::receiveFromPortThread, portSocket));
    portThreads.at(portThreads.size()-1).detach();
    
    if(UDP_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Created port %d\n", p->getPortId());
    }
}

void HardwareAODV::_destroyPort(Port* p){
    if(UDP_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Destroying port %d\n", p->getPortId());
    }
    if(portSockets.count(p->getPortId())){
        UDPSocket* portSocket = portSockets[p->getPortId()];
        portSockets.erase(p->getPortId());
        delete portSocket;
    }
}


void printPacket(FILE* file, char * buffer, int length){
    for(int i = 0; i < length; i++){
        fprintf(file, "%c", buffer[i]);
    }
}