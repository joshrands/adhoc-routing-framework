// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>
#include "hardware/hardware_aodv.h"

using namespace std;

void print_callback(char* data, int length){
    printf("[ADHOC]: ");
    for(int i = 0; i < length; i++){
        printf("%c", data[i]);
    }
    printf("\n");
}

int main(){
    // This might be a double negative right now...
//    ROUTING_PORT = 13415;
    HardwareAODV haodv(inet_addr("192.168.1.1"));
    haodv.setDataCallback(print_callback);

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
    }
}