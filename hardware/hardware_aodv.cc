#include "hardware_aodv.h"

// Helpers
void HardwareAODV::_hardwareAODV(){
    UDPSocket *aodvSocket = new UDPSocket();
    if (!aodvSocket->bindToPort(AODV_PORT)) {
        fprintf(stderr, "Could not bind the aodv socket to port:%d\n", AODV_PORT);
        exit(-1);
    }
    aodvSocket->setBroadcasting();
    thread aodving(&UDPSocket::receiveFromPortThread, aodvSocket);
    aodving.detach();

    UDPSocket *dataSocket = new UDPSocket();
    if (!dataSocket->bindToPort(DATA_PORT)) {
        fprintf(stderr, "Could not bind the data socket to port:%d\n", DATA_PORT);
        exit(-1);
    }
    dataSocket->setBroadcasting();
    thread dataing(&UDPSocket::receiveFromPortThread, dataSocket);
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

// Destructors
HardwareAODV::~HardwareAODV(){
}

// Override functions
int HardwareAODV::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port){
    if(port == AODV_PORT){
        aodvSocket->sendTo(buffer, length, dest, port);
    }else{
        dataSocket->sendTo(buffer, length, dest, port);
    }
}

// Class methods
void HardwareAODV::handleReceivedPackets(){
    Message message;
    while(aodvSocket->getMessage(message)){
        printf("Received: %s", message);
        decodeReceivedPacketBuffer(message.getData(), message.getLength(), message.getAddressI());
    }
}

bool HardwareAODV::getDataPacket(Message& message){
    return dataSocket->getMessage(message);
}