#include "hardware_aodv.h"

// Helpers
void HardwareAODV::_hardwareAODV(){
    //TODO: Only set aodv socket to broadcasting
    UDPSocket *aodvSocket = new UDPSocket();
    if (!aodvSocket->bindToPort(AODV_PORT)) {
        fprintf(stderr, "Could not bind the aodv socket to port:%d\n", AODV_PORT);
        exit(-1);
    }
    if(!aodvSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the aodv socket to broadcasting\n");
        exit(-1);
    }
    thread aodving(&UDPSocket::receiveFromPortThread, aodvSocket);
    aodving.detach();

    UDPSocket *dataSocket = new UDPSocket();
    if (!dataSocket->bindToPort(DATA_PORT)) {
        fprintf(stderr, "Could not bind the data socket to port:%d\n", DATA_PORT);
        exit(-1);
    }
    if(!dataSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the data socket to broadcasting\n");
        exit(-1);
    }
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

HardwareAODV::HardwareAODV(const char* ip) : AODV(ip) {
    _hardwareAODV();
}

// Destructors
HardwareAODV::~HardwareAODV(){
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