// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>
#include "hardware/hardware_aodv.h"

using namespace std;

int main(){
    // This might be a double negative right now...
    ROUTING_PORT = 13415;
    HardwareAODV haodv(inet_addr("192.168.1.1"));

    string message = "Hello World!";
    char buffer[200];
    strcpy(buffer, message.c_str());

    vector<string> ips = { 
        "192.168.1.2",
        "192.168.1.3"
        };

    while(true){
        // Send packets to all ips
        for(auto ip : ips){
            uint32_t dest = getIpFromString(ip);
            haodv.sendPacket(buffer, message.length(), dest);
            printf("Sent ");
            printPacket(stdout, buffer, message.length());
            printf(" to %s\n",ip);
        }
        // Handle packets
        int handleCount = haodv.handlePackets();
        if(handleCount > 0){
            printf("Handled %d AODV packets\n", handleCount);
        }
        // get data packets
        printf("Received data:\n");
        for(auto packet : haodv.getDataPackets()){
            printPacket(stdout, packet.getData(), packet.getLength());
            printf(" from %s\n", getStringFromIp(packet.getAddressI()));
        }
        

    }
}