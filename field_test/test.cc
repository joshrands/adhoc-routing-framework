// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>
#include "hardware/hardware_hello_aodv.h"
#include "aodv/test.h"

using namespace std;

int main(){
    RoutingProtocol* haodv = new HardwareHelloAODV("192.168.1.1");
    PrintPort* printPort = new PrintPort(DATA_PORT);
    haodv->addPort(printPort);

    string message = "Hello World!";
    char* msg = strdup(message.c_str());


    vector<string> ips = { 
        "192.168.1.2",
        "192.168.1.3"
    };

    while(true){
        // Send packets to all ips
        for(auto ip : ips){
            uint32_t dest = getIpFromString(ip);
            if(!haodv->sendPacket(printPort->getPortId(), msg, message.length(), dest)){
                fprintf(stderr, "[DO ADHOC]:[ERROR]: Muy angry send, much mad\n");
            }
            printf("[DO ADHOC]:[DEBUG]: Sent ");
            printPacket(stdout, msg, message.length());
            printf(" to %s\n", ip.c_str());
        }

        // Handle packets
        int handleCount = haodv->handlePackets();
        if(handleCount > 0){
            printf("[DO ADHOC]:[DEBUG]: Handled %d AODV packets\n", handleCount);
        }

        sleep(1);
    }

    // Clean memory up
    delete printPort;
    delete haodv;
}