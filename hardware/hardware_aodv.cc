#include "hardware_aodv.h"
#include <stdlib.h>
#include <iostream>

// Helpers
void HardwareAODV::_hardwareAODV(){
    //TODO: Only set aodv socket to broadcasting
    UDPSocket *aodvSocket = new UDPSocket();
    if (!aodvSocket->bindToPort(AODV_PORT)) {
        fprintf(stderr, "Could not bind the aodv socket to port:%d\n", AODV_PORT);

        std::cout << "Trying a new port..." << std::endl;
        // try a new port
        AODV_PORT = rand() % 10000; 
        // clean memory
        delete aodvSocket;
        // try again
        _hardwareAODV();

        return;
    }
    if(!aodvSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the aodv socket to broadcasting\n");
        exit(-1);
    }
    

    UDPSocket *dataSocket = new UDPSocket();
    if (!dataSocket->bindToPort(DATA_PORT)) {
        fprintf(stderr, "Could not bind the data socket to port:%d\n", DATA_PORT);

        std::cout << "Trying a new port..." << std::endl;
        // try a new port
        DATA_PORT = rand() % 10000; 
        // clean memory
        delete aodvSocket;
        // try again
        _hardwareAODV();

        return;
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
//    setIp(ip);
    _hardwareAODV();
}

HardwareAODV::HardwareAODV(uint32_t ip, int aodv_port, int data_port) : AODV(ip){
    UDPSocket *aodvSocket = new UDPSocket();
    if (!aodvSocket->bindToPort(aodv_port)) {
        fprintf(stderr, "Could not bind the aodv socket to port:%d\n", aodv_port);
        exit(-1);
    }
    if(!aodvSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the aodv socket to broadcasting\n");
        exit(-1);
    }
    aodving = thread(&UDPSocket::receiveFromPortThread, aodvSocket);
    

    UDPSocket *dataSocket = new UDPSocket();
    if (!dataSocket->bindToPort(data_port)) {
        fprintf(stderr, "Could not bind the data socket to port:%d\n", data_port);
        exit(-1);
    }
    if(!dataSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the data socket to broadcasting\n");
        exit(-1);
    }
    dataing = thread(&UDPSocket::receiveFromPortThread, dataSocket);

    aodving.detach();
    dataing.detach();
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
        return aodvSocket->sendTo(buffer, length, dest, port);
    }else{
        return dataSocket->sendTo(buffer, length, dest, port);
    }
}

// Class methods
int HardwareAODV::handleReceivedPackets(){
    Message message;
    int count = 0;
    while(aodvSocket->getMessage(message)){
        printf("Received: %s", message.getData());
        decodeReceivedPacketBuffer(message.getData(), message.getLength(), message.getAddressI());
        count ++;
    }
    return (count == 0) ? -1 : count;
}

bool HardwareAODV::getDataPacket(Message& message){
    return dataSocket->getMessage(message);
}